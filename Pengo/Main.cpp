#include <SDL3/SDL_main.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <ole2.h>
#endif

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "ResourceManager.h"

#include <filesystem>
#include <cstdlib>
#include <ctime>

//Sound
#include "Sound/Sound.h"
#include "ServiceLocator.h"
#include "Sound/SDLSoundSystem.h"

//State
#include "GameStateManager.h"
#include "ScoreManager.h"
#include "InputManager.h"
#include "States/StartMenuState.h"

namespace fs = std::filesystem;

//F2 Mute Command
namespace dae
{
	class ToggleMuteCommand final : public Command
	{
	public:
		ToggleMuteCommand() = default;
		virtual ~ToggleMuteCommand() override = default;

		virtual void Execute() override
		{
			auto& soundSystem = ServiceLocator::GetSoundSystem();

			if (!m_IsMuted)
			{
				m_previousVolume = soundSystem.GetVolume();

				soundSystem.SetVolume(0.0f);
				m_IsMuted = true;
			}
			else
			{
				soundSystem.SetVolume(m_previousVolume);
				m_IsMuted = false;
			}
		}

	private:
		bool m_IsMuted{ false };
		float m_previousVolume{ 1.0f };
	};
}


static void load()
{
	auto soundSystem = std::make_unique<dae::SDLSoundSystem>();
#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::LoggingSoundSystem>(std::move(soundSystem)));
#else
	dae::ServiceLocator::RegisterSoundSystem(std::move(soundSystem));
#endif

	dae::ScoreManager::GetInstance().Initialize();

	auto initialState = std::make_unique<dae::StartMenuState>();
	dae::GameStateManager::GetInstance().ChangeState(std::move(initialState));

	dae::InputManager::GetInstance().BindKey(
		SDL_SCANCODE_F2,
		dae::KeyState::Down,
		std::make_unique<dae::ToggleMuteCommand>()
	);
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "Data/";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif

	srand(static_cast<int>(time(0)));

	dae::Minigin engine(data_location);
	engine.Run(load);

    return 0;
}
