#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/FPSComponent.h"
#include "Components/RotationComponent.h"
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//Background
	auto gameObjectBackGround = std::make_unique<dae::GameObject>();
	gameObjectBackGround->AddComponent<dae::TextureComponent>();
	gameObjectBackGround->GetComponent<dae::TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(gameObjectBackGround));

	//Logo
	auto gameObjectLogo = std::make_unique<dae::GameObject>();
	gameObjectLogo->AddComponent<dae::TextureComponent>();
	gameObjectLogo->GetComponent<dae::TextureComponent>()->SetTexture("logo.png");
	gameObjectLogo->GetComponent<dae::TextureComponent>()->SetPosition(358, 180);
	scene.Add(std::move(gameObjectLogo));

	//Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto gameObjectText = std::make_unique<dae::GameObject>();
	gameObjectText->SetPosition(292, 20);
	gameObjectText->AddComponent<dae::TextureComponent>();
	gameObjectText->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	gameObjectText->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 0, 255 });
	
	scene.Add(std::move(gameObjectText));

	//FPS
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->AddComponent<dae::TextureComponent>();
	fpsObject->AddComponent<dae::TextComponent>("0 FPS", font);
	fpsObject->GetComponent<dae::TextComponent>()->SetColor({ 255, 255, 255, 255 });
	fpsObject->AddComponent<dae::FPSComponent>();
	fpsObject->SetPosition(10, 10);

	scene.Add(std::move(fpsObject));


	//Pengo's
	
//#define USE_ROTATOR

#ifdef USE_ROTATOR
	auto gameObjectRotator = std::make_unique<dae::GameObject>();
	gameObjectRotator->SetPosition(400, 400);

	auto gameObjectPengo1 = std::make_unique<dae::GameObject>();
	gameObjectPengo1->AddComponent<dae::TextureComponent>()->SetTexture("Pengo.png");
	gameObjectPengo1->SetParent(gameObjectRotator.get(), true);
	gameObjectPengo1->AddComponent<dae::RotationComponent>(20.f, 2.5f);

	scene.Add(std::move(gameObjectRotator));
#else
	auto gameObjectPengo1 = std::make_unique<dae::GameObject>();
	gameObjectPengo1->AddComponent<dae::TextureComponent>()->SetTexture("Pengo.png");

	const glm::vec3 offset{400.f, 400.f, 0.f};
	gameObjectPengo1->AddComponent<dae::RotationComponent>(20.f, 2.5f, offset);
#endif
	

	auto gameObjectPengo2 = std::make_unique<dae::GameObject>();
	gameObjectPengo2->AddComponent<dae::TextureComponent>();
	gameObjectPengo2->GetComponent<dae::TextureComponent>()->SetTexture("Pengo.png");

	gameObjectPengo2->SetParent(gameObjectPengo1.get(), true);
	gameObjectPengo2->AddComponent<dae::RotationComponent>(30.f, 5.f);

	scene.Add(std::move(gameObjectPengo1));
	scene.Add(std::move(gameObjectPengo2));
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
