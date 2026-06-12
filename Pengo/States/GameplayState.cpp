#include "GameplayState.h"
#include "StartMenuState.h"
#include "IntroState.h"
#include "HighScoreState.h"

#include "GameStateManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SnoBeeManager.h"
#include "PlayerManager.h"

#include "PengoCommands.h"
#include "Commands.h"

#include <memory>
#include <SDBMHasher.h>

#include "Components/ScoreComponent.h"
#include "Components/HealthComponent.h"

#include "GameTime.h"

#include <Events/EventManager.h>
#include <Events/Event.h>

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

    class SkipLevelCommand final : public Command
    {
    public:

        explicit SkipLevelCommand(GameplayState* pState, GameMode gameMode)
            : m_pState(pState)
            , m_GameMode(gameMode) {}
        
        void Execute() override
        {
            auto& snobeeManager = SnoBeeManager::GetInstance();
            snobeeManager.AddLevelIndex();

            int nextLevel = snobeeManager.GetLevelIndex();
            int totalLevels = LevelLoader::GetLevelCount(LevelLoader::GetLevelsPath());

            if (nextLevel < totalLevels)
            {
                GameStateManager::GetInstance().ChangeState(
                    std::make_unique<IntroState>(nextLevel, m_GameMode));
            }
            else
            {
                GameStateManager::GetInstance().ChangeState(
                    std::make_unique<HighScoreState>(m_pState->GetCombinedScore(), 0, GameEnding::GameWon));
            }
        }

    private:
        GameplayState* m_pState;
        GameMode m_GameMode;
    };
}

void dae::GameplayState::OnEnter()
{
    SetupInputBindings();

    m_LevelTimer = 0.0f;
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
    m_LevelTimer += GameTime::GetInstance().GetDeltaTime();

    //Check Player Death
    int combinedScore = 0;
    if (CheckAllPlayersDead(combinedScore))
    {
        return std::make_unique<HighScoreState>(
            combinedScore, 0,
            GameEnding::PlayerDied);
    }

    //Check Level Complete
    auto& snobeeManager = SnoBeeManager::GetInstance();
    if (snobeeManager.IsLevelComplete())
    {
        snobeeManager.AddLevelIndex();

        //DRY
        auto Keep = [&](GameObject* player, int idx)
            {
                if (!player) return;

                Event e(make_sdbm_hash("LevelFinish"));
                e.nbArgs = 1;
                e.args[0].gameObject = player;
                e.args[0].value = static_cast<int>(m_LevelTimer);
                EventManager::GetInstance().HandleEvent(e);

                if (auto* health = player->GetComponent<HealthComponent>())
                {
                    PlayerManager::GetInstance().SetLives(idx, health->GetLives());
                }
            };

        Keep(m_pPlayer1, 0);
        Keep(m_pPlayer2, 1);

        const int nextLevel = snobeeManager.GetLevelIndex();
        const int totalLevels = LevelLoader::GetLevelCount(LevelLoader::GetLevelsPath());
        
        if (nextLevel < totalLevels)
        {
            return std::make_unique<IntroState>(nextLevel, m_GameMode);
        }
        else
        {
            return std::make_unique<HighScoreState>(
                GetCombinedScore(), 0,
                GameEnding::GameWon);
        }
    }

    return nullptr;
}

bool dae::GameplayState::CheckAllPlayersDead(int& outScore) const
{
    //DRY
    auto isDead = [](GameObject* player) -> bool
        {
            if (!player) return true;
            auto* health = player->GetComponent<HealthComponent>();
            return health && health->GetLives() <= 0;
        };

    const bool p1Dead = isDead(m_pPlayer1);

    if (m_GameMode == GameMode::SinglePlayer)
    {
        if (p1Dead)
        {
            outScore = GetCombinedScore();
            return true;
        }
        return false;
    }


    const bool p2Dead = isDead(m_pPlayer2);
    if (p1Dead && p2Dead)
    {
        outScore = GetCombinedScore();
        return true;
    }
    return false;
}

int dae::GameplayState::GetCombinedScore() const
{
    int total = 0;
    
    //DRY
    auto Add = [&](GameObject* player)
        {
            if (!player) return;
            if (auto* s = player->GetComponent<ScoreComponent>())
            {
                total += s->GetScore();
            }
        };

    Add(m_pPlayer1);
    Add(m_pPlayer2);
    return total;
}

int dae::GameplayState::CalculateTimeBonus(float timeInSeconds) const
{
    int seconds = static_cast<int>(timeInSeconds);

    if (seconds >= 1 && seconds <= 20)   return 5000;
    if (seconds >= 21 && seconds <= 29)  return 2000;
    if (seconds >= 30 && seconds <= 39)  return 1000;
    if (seconds >= 40 && seconds <= 49)  return 500;
    if (seconds >= 50 && seconds <= 59)  return 10;

    return 0;
}

#pragma region InputBinding

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

        //F1 For Skip
        input.BindKey(SDL_SCANCODE_F1, KeyState::Down, std::make_unique<SkipLevelCommand>(this, m_GameMode));
    }


    if (m_GameMode == GameMode::SinglePlayer)
    {
        if (m_pPlayer1)
        {
            input.BindButton(0, ControllerButton::DpadUp, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0,-1 }));
            input.BindButton(0, ControllerButton::DpadDown, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0, 1 }));
            input.BindButton(0, ControllerButton::DpadLeft, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ -1, 0 }));
            input.BindButton(0, ControllerButton::DpadRight, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 1, 0 }));
            input.BindButton(0, ControllerButton::ButtonA, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer1));
        }
    }
    else // Multiplayer COOP/VS
    {
        if (m_pPlayer1)
        {
            input.BindButton(0, ControllerButton::DpadUp, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 0,-1 }));
            input.BindButton(0, ControllerButton::DpadDown, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 0, 1 }));
            input.BindButton(0, ControllerButton::DpadLeft, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ -1, 0 }));
            input.BindButton(0, ControllerButton::DpadRight, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer2, glm::ivec2{ 1, 0 }));

            input.BindButton(0, ControllerButton::ButtonA, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer2));
        }

        if (m_pPlayer2)
        {
            input.BindButton(1, ControllerButton::DpadUp, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0,-1 }));
            input.BindButton(1, ControllerButton::DpadDown, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 0, 1 }));
            input.BindButton(1, ControllerButton::DpadLeft, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ -1, 0 }));
            input.BindButton(1, ControllerButton::DpadRight, KeyState::Pressed, std::make_unique<MoveCommand>(m_pPlayer1, glm::ivec2{ 1, 0 }));
            input.BindButton(1, ControllerButton::ButtonA, KeyState::Pressed, std::make_unique<PushBlockCommand>(m_pPlayer1));
        }
    }

    //Quit
    input.BindKey(SDL_SCANCODE_ESCAPE, KeyState::Pressed, std::make_unique<PauseGameCommand>());
    input.BindButton(0, ControllerButton::Start, KeyState::Pressed, std::make_unique<PauseGameCommand>());
    input.BindButton(1, ControllerButton::Start, KeyState::Pressed, std::make_unique<PauseGameCommand>());
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

    if (m_GameMode != GameMode::SinglePlayer)
    {
        input.UnbindButton(1, ControllerButton::DpadUp, KeyState::Pressed);
        input.UnbindButton(1, ControllerButton::DpadDown, KeyState::Pressed);
        input.UnbindButton(1, ControllerButton::DpadLeft, KeyState::Pressed);
        input.UnbindButton(1, ControllerButton::DpadRight, KeyState::Pressed);
        input.UnbindButton(1, ControllerButton::ButtonA, KeyState::Pressed);
    }

    //Unbind Player 2
    input.UnbindButton(0, ControllerButton::DpadUp, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadDown, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadLeft, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::DpadRight, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::ButtonA, KeyState::Pressed);

    //Unbind Quit
    input.UnbindKey(SDL_SCANCODE_ESCAPE, KeyState::Pressed);
    input.UnbindButton(0, ControllerButton::Start, KeyState::Pressed);
    input.UnbindButton(1, ControllerButton::Start, KeyState::Pressed);

    //Unbind Skip
    input.UnbindKey(SDL_SCANCODE_F1, KeyState::Down);
}

#pragma endregion