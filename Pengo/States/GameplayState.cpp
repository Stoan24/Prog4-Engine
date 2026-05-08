#include "GameplayState.h"
#include "StartMenuState.h"

#include "SceneManager.h"
#include "InputManager.h"
#include "PengoCommands.h"
#include <Commands.h>

#include <memory>
#include <utility>

namespace dae
{
    class PauseGameCommand final : public Command
    {
    public:
        PauseGameCommand(GameplayState* pState) : m_pState(pState) {}

        void Execute() override
        {
            auto nextState = std::make_unique<StartMenuState>();

            m_pState->RequestStateChange(std::move(nextState));
        }

    private:
        GameplayState* m_pState;
    };
}

void dae::GameplayState::OnEnter()
{
    LoadLevel();
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

    return std::move(m_pPendingState);
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
        input.BindKey(SDL_SCANCODE_W, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0,-1 }));
        input.BindKey(SDL_SCANCODE_S, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0, 1 }));
        input.BindKey(SDL_SCANCODE_A, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ -1, 0 }));
        input.BindKey(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 1, 0 }));


        input.BindKey(SDL_SCANCODE_X, KeyState::Down, std::make_unique<KillEnemyCommand>(m_pPlayer1));
        input.BindKey(SDL_SCANCODE_E, KeyState::Down, std::make_unique<PushBlockCommand>(m_pPlayer1));
    }

    //Player 2 controller (Gamepad)
    if (m_pPlayer2)
    {
        input.BindButton(0, ControllerButton::DpadUp, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 0,-1 }));
        input.BindButton(0, ControllerButton::DpadDown, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 0, 1 }));
        input.BindButton(0, ControllerButton::DpadLeft, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ -1, 0 }));
        input.BindButton(0, ControllerButton::DpadRight, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 1, 0 }));


        input.BindButton(0, ControllerButton::ButtonX, KeyState::Pressed, std::make_unique<KillEnemyCommand>(m_pPlayer2));
        input.BindButton(0, ControllerButton::ButtonA, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer2));
    }

    input.BindKey(SDL_SCANCODE_ESCAPE, KeyState::Down, std::make_unique<PauseGameCommand>(this));
    input.BindButton(0, ControllerButton::Start, KeyState::Pressed, std::make_unique<PauseGameCommand>(this));
}

void dae::GameplayState::CleanupInputBindings()
{
    auto& input = InputManager::GetInstance();

    //Unbind Player 1
    input.UnbindKey(SDL_SCANCODE_W, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_S, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_A, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_D, KeyState::Pressed);
    input.UnbindKey(SDL_SCANCODE_X, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_E, KeyState::Down);

    //Unbind Player 2
    input.UnbindButton(0, ControllerButton::DpadUp, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadDown, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadLeft, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadRight, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::ButtonX, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::ButtonA, KeyState::Pressed);


    input.UnbindKey(SDL_SCANCODE_ESCAPE, KeyState::Down);
    input.UnbindButton(0, ControllerButton::Start, KeyState::Pressed);
}

void dae::GameplayState::LoadLevel()
{
    auto& sceneManager = SceneManager::GetInstance();
    m_pGameScene = &sceneManager.CreateScene("Game");
    sceneManager.SetActiveScene("Game");

    m_pGrid = m_LevelLoader.LoadLevel("Data/Levels/level1.json", *m_pGameScene);


    m_pPlayer1 = m_LevelLoader.GetObject("Pengo");
    m_pPlayer2 = m_LevelLoader.GetObject("Pengo2");
}
