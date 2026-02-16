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
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//Background
	auto goBG = std::make_unique<dae::GameObject>();
	auto TextureCompBG = goBG->AddComponent<dae::TextureComponent>();
	TextureCompBG->SetTexture("background.png");
	scene.Add(std::move(goBG));

	//Logo
	auto goLogo = std::make_unique<dae::GameObject>();
	auto TextureCompLogo = goLogo->AddComponent<dae::TextureComponent>();
	TextureCompLogo->SetTexture("logo.png");
	TextureCompLogo->SetPosition(358, 180);
	scene.Add(std::move(goLogo));

	//Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<dae::GameObject>();
	to->SetPosition(292, 20);
	
	auto textComp = to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	textComp->SetColor({ 255, 255, 0, 255 });
	
	scene.Add(std::move(to));

	//FPS

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
