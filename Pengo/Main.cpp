#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif


#include "Minigin.h"
#include "ResourceManager.h"

//input commands

#include <filesystem>

//Random
#include <cstdlib>
#include <ctime>

//Sound
#include "Sound/Sound.h"
#include "Sound/ServiceLocator.h"
#include "Sound/SDLSoundSystem.h"

//State
#include "GameStateManager.h"
#include "States/StartMenuState.h"

namespace fs = std::filesystem;

static void load()
{

#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::LoggingSoundSystem>(std::make_unique<dae::SDLSoundSystem>()));
#else
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
#endif

	auto initialState = std::make_unique<dae::StartMenuState>();
	dae::GameStateManager::GetInstance().ChangeState(std::move(initialState));
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
