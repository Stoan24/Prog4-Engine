#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"

//Components
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/FPSComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ThrashComponent.h"
#include "Transform.h"
#include "Scene.h"

//input commands
#include "InputManager.h"
#include "Commands.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();

	//Background
	auto gameObjectBackGround = std::make_unique<dae::GameObject>();
	gameObjectBackGround->AddComponent<dae::TextureComponent>();
	gameObjectBackGround->GetComponent<dae::TextureComponent>()->SetTexture("background.png");
	
	scene.Add(std::move(gameObjectBackGround));

	//Logo
	auto gameObjectLogo = std::make_unique<dae::GameObject>();
	gameObjectLogo->GetComponent<dae::Transform>()->SetLocalPosition(358, 180);
	gameObjectLogo->AddComponent<dae::TextureComponent>();
	gameObjectLogo->GetComponent<dae::TextureComponent>()->SetTexture("logo.png");

	scene.Add(std::move(gameObjectLogo));

	//Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto gameObjectText = std::make_unique<dae::GameObject>();
	gameObjectText->GetComponent<dae::Transform>()->SetLocalPosition(292, 20);
	gameObjectText->AddComponent<dae::TextureComponent>();
	gameObjectText->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	gameObjectText->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
	
	scene.Add(std::move(gameObjectText));

	//FPS
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->GetComponent<dae::Transform>()->SetLocalPosition(10, 10);
	fpsObject->AddComponent<dae::TextureComponent>();
	fpsObject->AddComponent<dae::TextComponent>("0 FPS", font);
	fpsObject->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
	fpsObject->AddComponent<dae::FPSComponent>();

	scene.Add(std::move(fpsObject));
	
	//Thrash the cache
	auto gameObjectGraphs = std::make_unique<dae::GameObject>();
	gameObjectGraphs->AddComponent<dae::ThrashComponent>();
	scene.Add(std::move(gameObjectGraphs));


	//Pengo's
	float baseSpeed = 100.f;


	auto KeyboardCharacter = std::make_unique<dae::GameObject>();
	KeyboardCharacter->AddComponent<dae::TextureComponent>()->SetTexture("Pengo.png");
	KeyboardCharacter->GetComponent<dae::Transform>()->SetLocalPosition(100, 100);


	input.BindKey(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(KeyboardCharacter.get(), glm::vec2{ 0, -1 }, baseSpeed));
	input.BindKey(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(KeyboardCharacter.get(), glm::vec2{ 0, 1 }, baseSpeed));
	input.BindKey(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(KeyboardCharacter.get(), glm::vec2{ -1, 0 }, baseSpeed));
	input.BindKey(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(KeyboardCharacter.get(), glm::vec2{ 1, 0 }, baseSpeed));

	scene.Add(std::move(KeyboardCharacter));



	auto controllerCharacter = std::make_unique<dae::GameObject>();
	controllerCharacter->AddComponent<dae::TextureComponent>()->SetTexture("Snobee.png");
	controllerCharacter->GetComponent<dae::Transform>()->SetLocalPosition(300, 100);

	const float doubleSpeed = baseSpeed * 2.0f;


	input.BindButton(0, dae::ControllerButton::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(controllerCharacter.get(), glm::vec2{ 0, -1 }, doubleSpeed));
	input.BindButton(0, dae::ControllerButton::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(controllerCharacter.get(), glm::vec2{ 0, 1 }, doubleSpeed));
	input.BindButton(0, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(controllerCharacter.get(), glm::vec2{ -1, 0 }, doubleSpeed));
	input.BindButton(0, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(controllerCharacter.get(), glm::vec2{ 1, 0 }, doubleSpeed));

	scene.Add(std::move(controllerCharacter));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
