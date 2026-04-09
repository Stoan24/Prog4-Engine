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
#include "Components/IceBlockComponent.h"
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

namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();
	dae::ScoreManager::GetInstance().Initialize();

	dae::LevelLoader loader;
	loader.LoadLevel("Data/Levels/level1.json", scene);

	auto* pengo = loader.GetObject("Pengo");
	auto* pengo2 = loader.GetObject("Pengo2");

	input.BindKey(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ 0,-1 }));
	input.BindKey(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ 0, 1 }));
	input.BindKey(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ -1, 0 }));
	input.BindKey(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo, glm::ivec2{ 1, 0 }));

	input.BindKey(SDL_SCANCODE_X, dae::KeyState::Down, std::make_unique<dae::KillEnemyCommand>(pengo));
	input.BindKey(SDL_SCANCODE_Y, dae::KeyState::Down, std::make_unique<dae::PushBlockCommand>(pengo));

	input.BindButton(0, dae::ControllerButton::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ 0,-1 }));
	input.BindButton(0, dae::ControllerButton::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ 0, 1 }));
	input.BindButton(0, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ -1, 0 }));
	input.BindButton(0, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(pengo2, glm::ivec2{ 1, 0 }));

	input.BindButton(0, dae::ControllerButton::ButtonX, dae::KeyState::Pressed, std::make_unique<dae::KillEnemyCommand>(pengo2));
	input.BindButton(0, dae::ControllerButton::ButtonY, dae::KeyState::Pressed, std::make_unique<dae::PushBlockCommand>(pengo2));


//	auto& scene = dae::SceneManager::GetInstance().CreateScene();
//	auto& input = dae::InputManager::GetInstance();
//	dae::ScoreManager::GetInstance().Initialize();
//
//#pragma region Dae Template
//	//Background
//	auto gameObjectBackGround = std::make_unique<dae::GameObject>();
//	gameObjectBackGround->AddComponent<dae::TextureComponent>();
//	gameObjectBackGround->GetComponent<dae::TextureComponent>()->SetTexture("background.png");
//	
//	scene.Add(std::move(gameObjectBackGround));
//
//	//Logo
//	auto gameObjectLogo = std::make_unique<dae::GameObject>();
//	gameObjectLogo->GetComponent<dae::Transform>()->SetLocalPosition(358, 180);
//	gameObjectLogo->AddComponent<dae::TextureComponent>();
//	gameObjectLogo->GetComponent<dae::TextureComponent>()->SetTexture("logo.png");
//
//	scene.Add(std::move(gameObjectLogo));
//
//	//Text
//	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
//	auto gameObjectText = std::make_unique<dae::GameObject>();
//	gameObjectText->GetComponent<dae::Transform>()->SetLocalPosition(292, 20);
//	gameObjectText->AddComponent<dae::TextureComponent>();
//	gameObjectText->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
//	gameObjectText->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
//	
//	scene.Add(std::move(gameObjectText));
//
//
//	//FPS
//	auto fpsObject = std::make_unique<dae::GameObject>();
//	fpsObject->GetComponent<dae::Transform>()->SetLocalPosition(10, 10);
//	fpsObject->AddComponent<dae::TextureComponent>();
//	fpsObject->AddComponent<dae::TextComponent>("0 FPS", font);
//	fpsObject->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
//	fpsObject->AddComponent<dae::FPSComponent>();
//
//	scene.Add(std::move(fpsObject));
//	
//	//Thrash the cache
//	//auto gameObjectGraphs = std::make_unique<dae::GameObject>();
//	//gameObjectGraphs->AddComponent<dae::ThrashComponent>();
//	//scene.Add(std::move(gameObjectGraphs));
//
//
//#pragma endregion Dae Template
//
//
//	auto controlsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
//
//
//#pragma region Level
//
//	auto gridObject = std::make_unique<dae::GameObject>();
//	gridObject->GetComponent<dae::Transform>()->SetLocalPosition(200, 200);
//	auto grid = gridObject->AddComponent<dae::GridComponent>(13, 15, 16);
//	gridObject->AddComponent<dae::TextureComponent>()->SetTexture("LevelBackground.png");
//
//	
//	auto makeIceBlock = [&](int col, int row)
//		{
//			auto block = std::make_unique<dae::GameObject>();
//			block->AddComponent<dae::TextureComponent>()->SetTexture("IceBlock.png");
//			block->AddComponent<dae::GridMoveComponent>(grid, col, row, 10.f);
//			block->AddComponent<dae::IceBlockComponent>(grid, col, row);
//			block->AddComponent<dae::CollisionComponent>()->SetSize(16, 16);
//
//			block->SetParent(gridObject.get(), false);
//
//			scene.Add(std::move(block));
//		};
//
//	makeIceBlock(2, 2);
//	makeIceBlock(10, 2);
//	makeIceBlock(5, 4);
//	makeIceBlock(7, 7);
//
//	
//
//#pragma endregion Level
//
//
//	//Players
//	const float moveSpeed{ 5.f };
//
//#pragma region Player1
//	auto Pengo = std::make_unique<dae::GameObject>();
//	Pengo->AddComponent<dae::TextureComponent>()->SetTexture("Pengo.png");
//	Pengo->AddComponent<dae::GridMoveComponent>(grid, 3, 3, moveSpeed);
//	Pengo->AddComponent<dae::CollisionComponent>()->SetSize(16, 16);
//	auto health = Pengo->AddComponent<dae::HealthComponent>(3);
//	auto score = Pengo->AddComponent<dae::ScoreComponent>();
//
//	Pengo->AddTag("Player");
//
//	//Input
//	input.BindKey(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo.get(), glm::ivec2{ 0, -1 }));
//	input.BindKey(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo.get(), glm::vec2{ 0, 1 }));
//	input.BindKey(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo.get(), glm::vec2{ -1, 0 }));
//	input.BindKey(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo.get(), glm::vec2{ 1, 0 }));
//
//	input.BindKey(SDL_SCANCODE_X, dae::KeyState::Down, std::make_unique<dae::KillEnemyCommand>(Pengo.get()));
//	input.BindKey(SDL_SCANCODE_Y, dae::KeyState::Down, std::make_unique<dae::PushBlockCommand>(Pengo.get()));
//
//
//	//Lives
//	auto livesDisplay = std::make_unique<dae::GameObject>();
//	livesDisplay->GetComponent<dae::Transform>()->SetLocalPosition(10, 200);
//	livesDisplay->AddComponent<dae::TextureComponent>();
//
//	auto livesText = livesDisplay->AddComponent<dae::TextComponent>("# Lives: 3", controlsFont);
//	livesDisplay->AddComponent<dae::HealthObserver>(livesText, health);
//	
//
//	//Score
//	auto scoreDisplay = std::make_unique<dae::GameObject>();
//	scoreDisplay->GetComponent<dae::Transform>()->SetLocalPosition(10, 220);
//	scoreDisplay->AddComponent<dae::TextureComponent>();
//
//	auto scoreText = scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", controlsFont);
//	scoreDisplay->AddComponent<dae::ScoreObserver>(scoreText, score);
//
//	Pengo->SetParent(gridObject.get(), false);
//
//	scene.Add(std::move(Pengo));
//	scene.Add(std::move(livesDisplay));
//	scene.Add(std::move(scoreDisplay));
//
//#pragma endregion Player1
//
//#pragma region Player2
//	auto Pengo1 = std::make_unique<dae::GameObject>();
//	Pengo1->AddComponent<dae::TextureComponent>()->SetTexture("Pengo.png");
//	Pengo1->AddComponent<dae::GridMoveComponent>(grid, 5, 10, moveSpeed * 2);
//	Pengo1->AddComponent<dae::CollisionComponent>()->SetSize(16, 16);
//	auto health1 = Pengo1->AddComponent<dae::HealthComponent>(3);
//	auto score1 = Pengo1->AddComponent<dae::ScoreComponent>();
//
//	Pengo1->AddTag("Player");
//
//	//Input
//	input.BindButton(0, dae::ControllerButton::DpadUp, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo1.get(), glm::vec2{ 0, -1 }));
//	input.BindButton(0, dae::ControllerButton::DpadDown, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo1.get(), glm::vec2{ 0, 1 }));
//	input.BindButton(0, dae::ControllerButton::DpadLeft, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo1.get(), glm::vec2{ -1, 0 }));
//	input.BindButton(0, dae::ControllerButton::DpadRight, dae::KeyState::Pressed, std::make_unique<dae::MoveCommand>(Pengo1.get(), glm::vec2{ 1, 0 }));
//
//	input.BindButton(0, dae::ControllerButton::ButtonX, dae::KeyState::Down, std::make_unique<dae::KillEnemyCommand>(Pengo1.get()));
//	input.BindButton(0, dae::ControllerButton::ButtonY, dae::KeyState::Down, std::make_unique<dae::PushBlockCommand>(Pengo1.get()));
//
//	//Lives
//	auto livesDisplay1 = std::make_unique<dae::GameObject>();
//	livesDisplay1->GetComponent<dae::Transform>()->SetLocalPosition(10, 240);
//	livesDisplay1->AddComponent<dae::TextureComponent>();
//
//	auto livesText1 = livesDisplay1->AddComponent<dae::TextComponent>("# Lives: 3", controlsFont);
//	livesDisplay1->AddComponent<dae::HealthObserver>(livesText1, health1);
//
//
//	//Score
//	auto scoreDisplay1 = std::make_unique<dae::GameObject>();
//	scoreDisplay1->GetComponent<dae::Transform>()->SetLocalPosition(10, 260);
//	scoreDisplay1->AddComponent<dae::TextureComponent>();
//
//	auto scoreText1 = scoreDisplay1->AddComponent<dae::TextComponent>("Score: 0", controlsFont);
//	scoreDisplay1->AddComponent<dae::ScoreObserver>(scoreText1, score1);
//
//	Pengo1->SetParent(gridObject.get(), false);
//
//	scene.Add(std::move(Pengo1));
//	scene.Add(std::move(livesDisplay1));
//	scene.Add(std::move(scoreDisplay1));
//
//#pragma endregion Player2
//
//#pragma region Enemy
//	auto Snobee = std::make_unique<dae::GameObject>();
//	Snobee->AddComponent<dae::TextureComponent>()->SetTexture("Snobee.png");
//	Snobee->GetComponent<dae::Transform>()->SetLocalPosition(350, 350);
//	Snobee->AddComponent<dae::CollisionComponent>()->SetSize(16, 16);
//	Snobee->AddComponent<dae::GridMoveComponent>(grid, 2, 13, moveSpeed * 2);
//
//	Snobee->AddTag("Enemy");
//
//	Snobee->SetParent(gridObject.get(), false);
//
//	scene.Add(std::move(Snobee));
//#pragma endregion Enemy
//
//#pragma region textObjects
//	//Keyboard Text
//	auto keyboardText = std::make_unique<dae::GameObject>();
//	keyboardText->GetComponent<dae::Transform>()->SetLocalPosition(10, 100);
//	keyboardText->AddComponent<dae::TextureComponent>();
//	keyboardText->AddComponent<dae::TextComponent>("Use WASD to move Yellow, collide with the SnoBee to lose a life", controlsFont);
//	keyboardText->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
//
//	scene.Add(std::move(keyboardText));
//
//	//Controller Text
//	auto controllerText = std::make_unique<dae::GameObject>();
//	controllerText->GetComponent<dae::Transform>()->SetLocalPosition(10, 120);
//	controllerText->AddComponent<dae::TextureComponent>();
//	controllerText->AddComponent<dae::TextComponent>("Use the D-Pad to move the Red, collide with the SnoBee to lose a life", controlsFont);
//	controllerText->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
//
//	scene.Add(std::move(controllerText));
//
//	//ScoreText
//	auto scoreInstructions = std::make_unique<dae::GameObject>();
//	scoreInstructions->GetComponent<dae::Transform>()->SetLocalPosition(10, 140);
//	scoreInstructions->AddComponent<dae::TextureComponent>();
//	scoreInstructions->AddComponent<dae::TextComponent>("Kill enemy(100) by pressing 'X' on controller or keyboard", controlsFont);
//	scoreInstructions->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
//	
//	scene.Add(std::move(scoreInstructions));
//
//	scoreInstructions = std::make_unique<dae::GameObject>();
//	scoreInstructions->GetComponent<dae::Transform>()->SetLocalPosition(10, 160);
//	scoreInstructions->AddComponent<dae::TextureComponent>();
//	scoreInstructions->AddComponent<dae::TextComponent>("Push block(50) by pressing 'Y' on controller or keyboard", controlsFont);
//	scoreInstructions->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
//	
//	scene.Add(std::move(scoreInstructions));
//#pragma endregion textObjects
//
//
//	scene.Add(std::move(gridObject));

}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "Data/";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif

	dae::Minigin engine(data_location);
	engine.Run(load);

    return 0;
}
