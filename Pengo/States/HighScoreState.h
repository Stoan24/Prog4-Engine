#pragma once
#include "GameState.h"
#include "StartMenuState.h"
#include <string>
#include <vector>
#include <Transform.h>

namespace dae
{
    class Scene;
    class GameObject;
    class TextComponent;

    enum class GameEnding
    {
        PlayerDied,
        GameWon
    };

    class HighScoreState final : public GameState
    {
    public:
        HighScoreState(int score, int playerIndex, GameEnding reason);
        ~HighScoreState() override = default;

        void OnEnter() override;
        void OnExit()  override;
        std::unique_ptr<GameState> Update() override;

        void CycleChar(int direction);
        void MoveCursor(int direction);
        void Confirm();

    private:
        int m_Score{};
        int m_PlayerIndex{};
        GameEnding m_Reason{};

        //Player data
        static constexpr int m_NameLength = 3;
        std::string m_CurrentName{ "OLI" };
        int m_CursorPos{ 0 };
        bool m_NameConfirmed{ false };


        Scene* m_pScene{ nullptr };

        std::vector<TextComponent*> m_CharTexts{};
        Transform* m_Cursor{ nullptr };


        //Helpers
        void BuildScene();
        void RefreshNameDisplay();

        void SetupInput();
        void CleanupInput();
    };
}