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

namespace dae
{
    class StartGameCommand final : public Command
    {
    public:

        void Execute() override
        {
            GameStateManager::GetInstance().ChangeState(std::make_unique<IntroState>());
        }
    };


    class QuitGameCommand final : public Command
    {
    public:
        void Execute() override
        {
        }
    };
}

void dae::StartMenuState::OnEnter()
{
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
    if (m_pMenuScene)
    {
        m_pMenuScene->Update();
    }

    return nullptr;
}

void dae::StartMenuState::Render()
{
    if (m_pMenuScene)
    {
        m_pMenuScene->Render();
    }
}


void dae::StartMenuState::SetupInputBindings()
{
    auto& input = InputManager::GetInstance();

    input.BindKey(SDL_SCANCODE_SPACE, KeyState::Down, std::make_unique<StartGameCommand>());
    input.BindButton(0, ControllerButton::ButtonA, KeyState::Down, std::make_unique<StartGameCommand>());

    input.BindKey(SDL_SCANCODE_ESCAPE, KeyState::Down, std::make_unique<QuitGameCommand>());
    input.BindButton(0, ControllerButton::Start, KeyState::Pressed, std::make_unique<QuitGameCommand>());
}

void dae::StartMenuState::CleanupInputBindings()
{
    auto& input = InputManager::GetInstance();


    input.UnbindKey(SDL_SCANCODE_SPACE, KeyState::Down);
    input.UnbindButton(0, ControllerButton::ButtonA, KeyState::Down);

    input.UnbindKey(SDL_SCANCODE_ESCAPE, KeyState::Down);
    input.UnbindButton(0, ControllerButton::Start, KeyState::Down);
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


    auto startObject = std::make_unique<GameObject>();
    auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
    startObject->GetComponent<Transform>()->SetLocalPosition(20.f, 150.f);
    startObject->AddComponent<TextureComponent>();
    startObject->AddComponent<TextComponent>("Press SPACE or A to Start", smallFont);
    m_pMenuScene->Add(std::move(startObject));


    auto daeObject = std::make_unique<GameObject>();
    daeObject->GetComponent<Transform>()->SetLocalPosition(76.f, 200.f);
    daeObject->AddComponent<TextureComponent>()->SetTexture("UI/Company.png");
    m_pMenuScene->Add(std::move(daeObject));
}
