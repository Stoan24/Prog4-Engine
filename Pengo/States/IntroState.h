#pragma once
#include "GameState.h"
#include "LevelLoader.h"
#include "Components/GridComponent.h"
#include "StartMenuState.h"

namespace dae
{
    class Scene;
    class GameObject;

    class IntroState final : public GameState
    {
    public:

        explicit IntroState(int levelIndex = 0, GameMode gameMode = GameMode::SinglePlayer)
            :m_LevelIndex(levelIndex),
            m_GameMode(gameMode)
        {}
        ~IntroState() override = default;

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<dae::GameState> Update() override;
        void Render() override;

    private:

        int m_LevelIndex{ 0 };
        GameMode m_GameMode{ GameMode::SinglePlayer };

        LevelLoader m_LevelLoader;
        Scene* m_pGameScene{ nullptr };
        GridComponent* m_pGrid{ nullptr };
        GameObject* m_pPlayer1{ nullptr };
        GameObject* m_pPlayer2{ nullptr };

        float m_IntroTimer{ 3.f };
    };
}