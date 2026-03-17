#include <stdexcept>
#include <sstream>
#include <iostream>

#include <chrono>
#include <thread>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Events/EventManager.h"

#include "GameTime.h"

#ifdef USE_STEAMWORKS

#include <Steam/Achievement.h>
#endif // USE_STEAMWORKS



SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"



void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

dae::Minigin::Minigin(const std::filesystem::path& dataPath)
{
	PrintSDLVersion();
	
#ifndef __EMSCRIPTEN__
	//Emscirpten does not like double initialisation
	if (!(SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO))
	{
		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		{
			SDL_Log("Renderer error: %s", SDL_GetError());
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
		}
	}
#endif

	g_window = SDL_CreateWindow(
		"Pengo",
		1024,
		576,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
#if USE_STEAMWORKS
	if (!SteamAPI_Init())
		throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
	g_SteamAchievements = new SteamAchievements(g_Achievements, 4);
#endif

	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
#if USE_STEAMWORKS
	delete g_SteamAchievements;
	SteamAPI_Shutdown();
#endif

	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	m_LastTime = std::chrono::high_resolution_clock::now();

#ifndef __EMSCRIPTEN__
	while (!m_quit)
		RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif

	SceneManager::GetInstance().Clear();
	Renderer::GetInstance().Destroy();
}

void dae::Minigin::RunOneFrame()
{
#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif

	const std::chrono::duration<float> desiredFrameTime{ 1.f / m_desiredFPS };
	
	//Calculate delta time
	const auto currentTime{ std::chrono::high_resolution_clock::now() };
	const std::chrono::duration<float> deltaTime{ currentTime - m_LastTime };

	GameTime::GetInstance().SetDeltaTime(deltaTime.count());
	m_LastTime = currentTime;


	//Game loop
	m_quit = !InputManager::GetInstance().ProcessInput();
	SceneManager::GetInstance().Update();
	EventManager::GetInstance().Update();
	Renderer::GetInstance().Render();
	
	const auto frameEnd = std::chrono::high_resolution_clock::now();
	const auto frameDuration = frameEnd - currentTime;
	const auto SleepTime = desiredFrameTime - frameDuration;

	if (SleepTime.count() > 0)
	{
		std::this_thread::sleep_for(SleepTime);
	}
}
