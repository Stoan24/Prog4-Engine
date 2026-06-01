#include "GameplayState.h"
#include "StartMenuState.h"
#include "GameStateManager.h"

#include "SceneManager.h"
#include "InputManager.h"
#include "SnoBeeManager.h"
#include "PlayerManager.h"

#include "PengoCommands.h"
#include "Commands.h"

#include <memory>
#include "IntroState.h"
#include "Components/ScoreComponent.h"
#include "Components/HealthComponent.h"

namespace dae
{
    class PauseGameCommand final : public Command
    {
    public:

        void Execute() override
        {
            GameStateManager::GetInstance().ChangeState(std::make_unique<StartMenuState>());
        }
    };
}

void dae::GameplayState::OnEnter()
{
    SetupInputBindings();
}

void dae::GameplayState::OnExit()
{
    CleanupInputBindings();

    auto& sceneManager = SceneManager::GetInstance();
    if (m_pGameScene)
    {
        sceneManager.Clear();
        m_pGameScene = nullptr;
        m_pGrid = nullptr;
        m_pPlayer1 = nullptr;
        m_pPlayer2 = nullptr;
    }
}

std::unique_ptr<dae::GameState> dae::GameplayState::Update()
{
    if (m_pGameScene)
    {
        m_pGameScene->Update();
    }

    if (SnoBeeManager::GetInstance().IsLevelComplete())
    {
        SnoBeeManager::GetInstance().AddLevelIndex();

        if (m_pPlayer1)
        {
            auto* score = m_pPlayer1->GetComponent<ScoreComponent>();
            auto* health = m_pPlayer1->GetComponent<HealthComponent>();
            if (score)  PlayerManager::GetInstance().SetScore(0, score->GetScore());
            if (health) PlayerManager::GetInstance().SetLives(0, health->GetLives());
        }
        if (m_pPlayer2)
        {
            auto* score = m_pPlayer2->GetComponent<ScoreComponent>();
            auto* health = m_pPlayer2->GetComponent<HealthComponent>();
            if (score)  PlayerManager::GetInstance().SetScore(1, score->GetScore());
            if (health) PlayerManager::GetInstance().SetLives(1, health->GetLives());
        }

        int nextLevel = SnoBeeManager::GetInstance().GetLevelIndex();
        if (nextLevel < LevelLoader::GetLevelCount(LevelLoader::GetLevelsPath()))
        {
            return std::make_unique<IntroState>(nextLevel, m_GameMode);
        }
    }

    return nullptr;
}

void dae::GameplayState::Render()
{
    if (m_pGameScene)
    {
        m_pGameScene->Render();
    }
}

void dae::GameplayState::SetupInputBindings()
{
    auto& input = InputManager::GetInstance();

    //Player 1 keyboard (WASD)
    if (m_pPlayer1)
    {
        //WASD
        input.BindKey(SDL_SCANCODE_W, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0,-1 }));
        input.BindKey(SDL_SCANCODE_S, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0, 1 }));
        input.BindKey(SDL_SCANCODE_A, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ -1, 0 }));
        input.BindKey(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 1, 0 }));

        //Arrows
        input.BindKey(SDL_SCANCODE_UP, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0,-1 }));
        input.BindKey(SDL_SCANCODE_DOWN, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0, 1 }));
        input.BindKey(SDL_SCANCODE_LEFT, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ -1, 0 }));
        input.BindKey(SDL_SCANCODE_RIGHT, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 1, 0 }));


        input.BindKey(SDL_SCANCODE_E, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer1));

        if (m_GameMode == GameMode::SinglePlayer)
        {
            input.BindButton(0, ControllerButton::DpadUp, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0,-1 }));
            input.BindButton(0, ControllerButton::DpadDown, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0, 1 }));
            input.BindButton(0, ControllerButton::DpadLeft, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ -1, 0 }));
            input.BindButton(0, ControllerButton::DpadRight, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 1, 0 }));
            input.BindButton(0, ControllerButton::ButtonA, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer1));
        }
    }

    //Player 2 controller (Gamepad)
    if (m_pPlayer2 && m_GameMode != GameMode::SinglePlayer)
    {
        input.BindButton(0, ControllerButton::DpadUp, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 0,-1 }));
        input.BindButton(0, ControllerButton::DpadDown, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 0, 1 }));
        input.BindButton(0, ControllerButton::DpadLeft, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ -1, 0 }));
        input.BindButton(0, ControllerButton::DpadRight, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 1, 0 }));

        if (m_GameMode == GameMode::Coop)
        {
            input.BindButton(0, ControllerButton::ButtonA, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer2));
        }
    }

    //Quit
    input.BindKey(SDL_SCANCODE_ESCAPE, KeyState::Pressed, std::make_unique<PauseGameCommand>());
    input.BindButton(0, ControllerButton::Start, KeyState::Pressed, std::make_unique<PauseGameCommand>());
}

void dae::GameplayState::CleanupInputBindings()
{
    auto& input = InputManager::GetInstance();

    //Unbind Player 1
    input.UnbindKey(SDL_SCANCODE_W, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_S, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_A, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_D, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_UP, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_DOWN, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_LEFT, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_RIGHT, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_E, KeyState::Pressed);

    //Unbind Player 2
    input.UnbindButton(0, ControllerButton::DpadUp, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadDown, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadLeft, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadRight, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::ButtonA, KeyState::Pressed);


    input.UnbindKey(SDL_SCANCODE_ESCAPE, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::Start, KeyState::Pressed);
}
