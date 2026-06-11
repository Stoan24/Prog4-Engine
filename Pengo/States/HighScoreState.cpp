#include "HighScoreState.h"
#include "StartMenuState.h"
#include "GameStateManager.h"
#include "HighScoreManager.h"

#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Events/EventManager.h"

#include "GameObject.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Transform.h"
#include "Commands.h"

#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDBMHasher.h>


namespace dae
{
    class HSCycleUpCommand final : public Command
    {
    public:
        explicit HSCycleUpCommand(HighScoreState* s) : m_State(s) {}
        void Execute() override { m_State->CycleChar(+1); }
    private:
        HighScoreState* m_State;
    };

    class HSCycleDownCommand final : public Command
    {
    public:
        explicit HSCycleDownCommand(HighScoreState* s) : m_State(s) {}
        void Execute() override { m_State->CycleChar(-1); }
    private:
        HighScoreState* m_State;
    };

    class HSCursorLeftCommand final : public Command
    {
    public:
        explicit HSCursorLeftCommand(HighScoreState* s) : m_State(s) {}
        void Execute() override { m_State->MoveCursor(-1); }
    private:
        HighScoreState* m_State;
    };

    class HSCursorRightCommand final : public Command
    {
    public:
        explicit HSCursorRightCommand(HighScoreState* s) : m_State(s) {}
        void Execute() override { m_State->MoveCursor(+1); }
    private:
        HighScoreState* m_State;
    };

    class HSConfirmCommand final : public Command
    {
    public:
        explicit HSConfirmCommand(HighScoreState* s) : m_State(s) {}
        void Execute() override { m_State->Confirm(); }
    private:
        HighScoreState* m_State;
    };
}


dae::HighScoreState::HighScoreState(int score, int playerIndex, GameEnding reason)
    : m_Score(score)
    , m_PlayerIndex(playerIndex)
    , m_Reason(reason)
{
}

void dae::HighScoreState::OnEnter()
{
    HighScoreManager::GetInstance().Load();

    auto& sm = SceneManager::GetInstance();
    m_pScene = &sm.CreateScene("HighScore");
    sm.SetActiveScene("HighScore");

    BuildScene();
    SetupInput();
}

void dae::HighScoreState::OnExit()
{
    CleanupInput();
}

std::unique_ptr<dae::GameState> dae::HighScoreState::Update()
{
    if (!m_NameConfirmed)
    {
        return nullptr;
    }


    if (m_PlayerIndex >= 0)
    {
        HighScoreManager::GetInstance().AddEntry(m_CurrentName, m_Score);
    }

    return std::make_unique<StartMenuState>();
}


void dae::HighScoreState::BuildScene()
{
    auto bigFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 28);
    auto medFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
    auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);

    //Background
    auto bg = std::make_unique<GameObject>();
    bg->GetComponent<Transform>()->SetLocalPosition(0.f, 0.f);
    bg->AddComponent<TextureComponent>()->SetTexture("mainBackground.png");
    m_pScene->Add(std::move(bg));


    //Title
    auto title = std::make_unique<GameObject>();
    title->GetComponent<Transform>()->SetLocalPosition(55.f, 10.f);
    title->AddComponent<TextureComponent>();
    title->AddComponent<TextComponent>("PENGO", medFont);
    m_pScene->Add(std::move(title));

    //GameOverText
    const char* reasonText = (m_Reason == GameEnding::GameWon)
        ? "You won! Enter your name:"
        : "Game Over! Enter your name:";

    auto label = std::make_unique<GameObject>();
    label->GetComponent<Transform>()->SetLocalPosition(20.f, 40.f);
    label->AddComponent<TextureComponent>();
    label->AddComponent<TextComponent>(reasonText, smallFont);
    m_pScene->Add(std::move(label));

    //Score
    auto scoreLabel = std::make_unique<GameObject>();
    scoreLabel->GetComponent<Transform>()->SetLocalPosition(20.f, 58.f);
    scoreLabel->AddComponent<TextureComponent>();
    scoreLabel->AddComponent<TextComponent>("Score: " + std::to_string(m_Score), smallFont);
    m_pScene->Add(std::move(scoreLabel));

    //Name
    m_CharTexts.resize(m_NameLength, nullptr);

    for (int i = 0; i < m_NameLength; ++i)
    {
        float x = 60.f + i * 30.f;

        auto charObj = std::make_unique<GameObject>();
        charObj->GetComponent<Transform>()->SetLocalPosition(x, 85.f);
        charObj->AddComponent<TextureComponent>();
        m_CharTexts[i] = charObj->AddComponent<TextComponent>(std::string(1, m_CurrentName[i]), bigFont);
        m_pScene->Add(std::move(charObj));
    }

    //Cursor
    auto cursorObj = std::make_unique<GameObject>();
    m_Cursor = cursorObj->GetComponent<Transform>();
    m_Cursor->SetLocalPosition(60.f + m_CursorPos * 30.f, 115.f);
    cursorObj->AddComponent<TextureComponent>();
    cursorObj->AddComponent<TextComponent>("^", smallFont);
    m_pScene->Add(std::move(cursorObj));

    //Controls
    auto letterControls = std::make_unique<GameObject>();
    letterControls->GetComponent<Transform>()->SetLocalPosition(20.f, 130.f);
    letterControls->AddComponent<TextureComponent>();
    letterControls->AddComponent<TextComponent>("Up/Down: change letter", smallFont);
    m_pScene->Add(std::move(letterControls));

    auto cursorControls = std::make_unique<GameObject>();
    cursorControls->GetComponent<Transform>()->SetLocalPosition(20.f, 150.f);
    cursorControls->AddComponent<TextureComponent>();
    cursorControls->AddComponent<TextComponent>("Left/Right: move", smallFont);
    m_pScene->Add(std::move(cursorControls));

    auto confirmControls = std::make_unique<GameObject>();
    confirmControls->GetComponent<Transform>()->SetLocalPosition(20.f, 170.f);
    confirmControls->AddComponent<TextureComponent>();
    confirmControls->AddComponent<TextComponent>("A/Enter: confirm", smallFont);
    m_pScene->Add(std::move(confirmControls));


    //Leaderboard
    auto header = std::make_unique<GameObject>();
    header->GetComponent<Transform>()->SetLocalPosition(20.f, 195.f);
    header->AddComponent<TextureComponent>();
    header->AddComponent<TextComponent>("LeaderBoard", smallFont);
    m_pScene->Add(std::move(header));

    const auto& entries = HighScoreManager::GetInstance().GetEntries();
    for (int i = 0; i < static_cast<int>(entries.size()) && i < 10; ++i)
    {
        std::ostringstream oss;
        oss << (i + 1) << ". " << entries[i].name
            << "  " << std::setw(6) << entries[i].score;

        auto row = std::make_unique<GameObject>();
        row->GetComponent<Transform>()->SetLocalPosition(20.f, 210.f + i * 16.f);
        row->AddComponent<TextureComponent>();
        row->AddComponent<TextComponent>(oss.str(), smallFont);
        m_pScene->Add(std::move(row));
    }
}

void dae::HighScoreState::RefreshNameDisplay()
{
    for (int i = 0; i < m_NameLength; ++i)
    {
        if (m_CharTexts[i])
        {
            m_CharTexts[i]->SetText(std::string(1, m_CurrentName[i]));
        }

        if (m_Cursor)
        {
            m_Cursor->SetLocalPosition(60.f + m_CursorPos * 30.f, 115.f);
        }
    }
}

void dae::HighScoreState::CycleChar(int direction)
{
    char& c = m_CurrentName[m_CursorPos];
    c = static_cast<char>(c + direction);
    
    //Looping
    if (c > 'Z') c = 'A';
    if (c < 'A') c = 'Z';
    RefreshNameDisplay();
}

void dae::HighScoreState::MoveCursor(int direction)
{
    m_CursorPos = (m_CursorPos + direction + m_NameLength) % m_NameLength;
    RefreshNameDisplay();
}

void dae::HighScoreState::Confirm()
{
    m_NameConfirmed = true;

    Event e(make_sdbm_hash("GameReset"));
    EventManager::GetInstance().HandleEvent(e);
}

#pragma region InputBinding

void dae::HighScoreState::SetupInput()
{
    auto& input = InputManager::GetInstance();

    //Controller
    input.BindButton(0, ControllerButton::DpadUp, KeyState::Down, std::make_unique<HSCycleUpCommand>(this));
    input.BindButton(0, ControllerButton::DpadDown, KeyState::Down, std::make_unique<HSCycleDownCommand>(this));
    input.BindButton(0, ControllerButton::DpadLeft, KeyState::Down, std::make_unique<HSCursorLeftCommand>(this));
    input.BindButton(0, ControllerButton::DpadRight, KeyState::Down, std::make_unique<HSCursorRightCommand>(this));
    input.BindButton(0, ControllerButton::ButtonA, KeyState::Down, std::make_unique<HSConfirmCommand>(this));

    //Keyboard
    input.BindKey(SDL_SCANCODE_UP, KeyState::Down, std::make_unique<HSCycleUpCommand>(this));
    input.BindKey(SDL_SCANCODE_DOWN, KeyState::Down, std::make_unique<HSCycleDownCommand>(this));
    input.BindKey(SDL_SCANCODE_LEFT, KeyState::Down, std::make_unique<HSCursorLeftCommand>(this));
    input.BindKey(SDL_SCANCODE_RIGHT, KeyState::Down, std::make_unique<HSCursorRightCommand>(this));
    input.BindKey(SDL_SCANCODE_W, KeyState::Down, std::make_unique<HSCycleUpCommand>(this));
    input.BindKey(SDL_SCANCODE_S, KeyState::Down, std::make_unique<HSCycleDownCommand>(this));
    input.BindKey(SDL_SCANCODE_A, KeyState::Down, std::make_unique<HSCursorLeftCommand>(this));
    input.BindKey(SDL_SCANCODE_D, KeyState::Down, std::make_unique<HSCursorRightCommand>(this));

    input.BindKey(SDL_SCANCODE_RETURN, KeyState::Down, std::make_unique<HSConfirmCommand>(this));
}

void dae::HighScoreState::CleanupInput()
{
    auto& input = InputManager::GetInstance();

    input.UnbindButton(0, ControllerButton::DpadUp, KeyState::Down);
    input.UnbindButton(0, ControllerButton::DpadDown, KeyState::Down);
    input.UnbindButton(0, ControllerButton::DpadLeft, KeyState::Down);
    input.UnbindButton(0, ControllerButton::DpadRight, KeyState::Down);
    input.UnbindButton(0, ControllerButton::ButtonA, KeyState::Down);


    input.UnbindKey(SDL_SCANCODE_UP, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_DOWN, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_LEFT, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_RIGHT, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_W, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_S, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_A, KeyState::Down);
    input.UnbindKey(SDL_SCANCODE_D, KeyState::Down);

    input.UnbindKey(SDL_SCANCODE_RETURN, KeyState::Down);
}

#pragma endregion