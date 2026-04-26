#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif


#include "Minigin.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"

//Components
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/FPSComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ThrashComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/GridComponent.h"
#include "Components/GridMoveComponent.h"
#include "Transform.h"

//Observers
#include "Observers/HealthObserver.h"
#include "Observers/ScoreObserver.h"

//Events
#include "ScoreManager.h"

//input commands
#include "InputManager.h"
#include "PengoCommands.h"

#include <filesystem>
#include "LevelLoader.h"

//Random
#include <cstdlib>
#include <ctime>

//Sound
#include "Sound/Sound.h"
#include "Sound/ServiceLocator.h"
#include "Sound/SDLSoundSystem.h"

namespace fs = std::filesystem;

static void load()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	auto& ss = dae::ServiceLocator::GetSoundSystem();

	ss.LoadSound(0, "Data/Sounds/ActStart.mp3");
	ss.Play(0, 2);


	auto& gameScene = dae::SceneManager::GetInstance().CreateScene("Game");
	dae::SceneManager::GetInstance().SetActiveScene("Game");
	auto& input = dae::InputManager::GetInstance();
	dae::ScoreManager::GetInstance().Initialize();

	dae::LevelLoader loader;
	loader.LoadLevel("Data/Levels/level1.json", gameScene);

	auto* pengo = loader.GetObject("Pengo");
	auto* pengo2 = loader.GetObject("Pengo2");

	input.BindKey(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ 0,-1 }));
	input.BindKey(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ 0, 1 }));
	input.BindKey(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ -1, 0 }));
	input.BindKey(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ 1, 0 }));

	input.BindKey(SDL_SCANCODE_X, dae::KeyState::Down, std::make_unique<dae::KillEnemyCommand>(pengo));
	input.BindKey(SDL_SCANCODE_E, dae::KeyState::Down, std::make_unique<dae::PushBlockCommand>(pengo));

	input.BindButton(0, dae::ControllerButton::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ 0,-1 }));
	input.BindButton(0, dae::ControllerButton::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ 0, 1 }));
	input.BindButton(0, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ -1, 0 }));
	input.BindButton(0, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ 1, 0 }));

	input.BindButton(0, dae::ControllerButton::ButtonX, dae::KeyState::Pressed, std::make_unique<dae::KillEnemyCommand>(pengo2));
	input.BindButton(0, dae::ControllerButton::ButtonA, dae::KeyState::Pressed, std::make_unique<dae::PushBlockCommand>(pengo2));

	
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
