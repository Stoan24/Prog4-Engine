#include "StartMenuState.h"
#include "GameStateManager.h"
#include "IntroState.h"

#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "PlayerManager.h"

#include "GameObject.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Transform.h"

#include "Commands.h"

#include <utility>
#include <memory>


#pragma region Commands
namespace dae
{
    class QuitGameCommand final : public Command
    {
    public:
        void Execute() override
        {
        }
    };

    class StartSinglePlayerCommand final : public Command
    {
    public:
        void Execute() override
        {
            PlayerManager::GetInstance().Initialize(1);
            GameStateManager::GetInstance().ChangeState(std::make_unique<IntroState>(0, GameMode::SinglePlayer));
        }
    };

    class StartCoopCommand final : public Command
    {
    public:
        void Execute() override
        {
            PlayerManager::GetInstance().Initialize(2);
            GameStateManager::GetInstance().ChangeState(std::make_unique<IntroState>(0, GameMode::Coop));
        }
    };

    class StartVersusCommand final : public Command
    {
    public:
        void Execute() override
        {
            PlayerManager::GetInstance().Initialize(2);
            GameStateManager::GetInstance().ChangeState(std::make_unique<IntroState>(0, GameMode::Versus));
        }
    };
}
#pragma endregion

void dae::StartMenuState::OnEnter()
{
    LevelLoader::LoadSounds("Data/JSON/sounds.json");
    PlayerManager::GetInstance().Initialize(2);

    CreateMenuScene();
    SetupInputBindings();
}

void dae::StartMenuState::OnExit()
{
    CleanupInputBindings();
}

std::unique_ptr<dae::GameState> dae::StartMenuState::Update()
{
    return nullptr;
}

void dae::StartMenuState::CreateMenuScene()
{
    auto& sceneManager = SceneManager::GetInstance();


    m_pMenuScene = &sceneManager.CreateScene("MainMenu");
    sceneManager.SetActiveScene("MainMenu");

    auto backgroundObject = std::make_unique<GameObject>();
    backgroundObject->GetComponent<Transform>()->SetLocalPosition(0.f, 0.f);
    backgroundObject->AddComponent<TextureComponent>()->SetTexture("mainBackground.png");
    m_pMenuScene->Add(std::move(backgroundObject));


    auto titleObject = std::make_unique<GameObject>();
    titleObject->GetComponent<Transform>()->SetLocalPosition(40.f, 25.f);
    titleObject->AddComponent<TextureComponent>()->SetTexture("UI/Title.png");
    m_pMenuScene->Add(std::move(titleObject));


    auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);

    auto singleplayerText = std::make_unique<GameObject>();
    singleplayerText->GetComponent<Transform>()->SetLocalPosition(20.f, 150.f);
    singleplayerText->AddComponent<TextureComponent>();
    singleplayerText->AddComponent<TextComponent>("Press 1 or A for SinglePlayer", smallFont);
    m_pMenuScene->Add(std::move(singleplayerText));

    auto coopText = std::make_unique<GameObject>();
    coopText->GetComponent<Transform>()->SetLocalPosition(20.f, 170.f);
    coopText->AddComponent<TextureComponent>();
    coopText->AddComponent<TextComponent>("Press 2 or X for COOP", smallFont);
    m_pMenuScene->Add(std::move(coopText));

    auto versusText = std::make_unique<GameObject>();
    versusText->GetComponent<Transform>()->SetLocalPosition(20.f, 190.f);
    versusText->AddComponent<TextureComponent>();
    versusText->AddComponent<TextComponent>("Press 3 or Y for Versus", smallFont);
    m_pMenuScene->Add(std::move(versusText));


    auto daeObject = std::make_unique<GameObject>();
    daeObject->GetComponent<Transform>()->SetLocalPosition(76.f, 250.f);
    daeObject->AddComponent<TextureComponent>()->SetTexture("UI/Company.png");
    m_pMenuScene->Add(std::move(daeObject));
}

#pragma region InputBindings
void dae::StartMenuState::SetupInputBindings()
{
    auto& input = InputManager::GetInstance();

    input.BindKey(SDL_SCANCODE_1, KeyState::Down, std::make_unique<StartSinglePlayerCommand>());
    input.BindKey(SDL_SCANCODE_2, KeyState::Down, std::make_unique<StartCoopCommand>());
    input.BindKey(SDL_SCANCODE_3, KeyState::Down, std::make_unique<StartVersusCommand>());
    input.BindButton(0, ControllerButton::ButtonA, KeyState::Down, std::make_unique<StartSinglePlayerCommand>());
    input.BindButton(0, ControllerButton::ButtonX, KeyState::Down, std::make_unique<StartCoopCommand>());
    input.BindButton(0, ControllerButton::ButtonY, KeyState::Down, std::make_unique<StartVersusCommand>());

    input.BindKey(SDL_SCANCODE_ESCAPE, KeyState::Down, std::make_unique<QuitGameCommand>());
    input.BindButton(0, ControllerButton::Start, KeyState::Pressed, std::make_unique<QuitGameCommand>());
}

void dae::StartMenuState::CleanupInputBindings()
{
    auto& input = InputManager::GetInstance();

    input.UnbindKey(SDL_SCANCODE_1, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_2, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_3, KeyState::Down);
    input.UnbindButton(0, ControllerButton::ButtonA, KeyState::Down);
    input.UnbindButton(0, ControllerButton::ButtonX, KeyState::Down);
    input.UnbindButton(0, ControllerButton::ButtonY, KeyState::Down);

    input.UnbindKey(SDL_SCANCODE_ESCAPE, KeyState::Down);
    input.UnbindButton(0, ControllerButton::Start, KeyState::Down);
}
#pragma endregion