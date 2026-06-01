#pragma once
#include "GameState.h"
#include "LevelLoader.h"
#include "Components/GridComponent.h"
#include <memory>

namespace dae
{
    class Scene;
    class GameObject;

    class GameplayState final : public GameState
    {
    public:
        GameplayState(Scene* scene, GridComponent* grid, GameObject* player1, GameObject* player2, GameMode gameMode)
            :m_GameMode(gameMode),
            m_pGameScene(scene),
            m_pGrid(grid),
            m_pPlayer1(player1),
            m_pPlayer2(player2)
        {
        }

        ~GameplayState() override = default;

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<dae::GameState> Update() override;
        void Render() override;


    private:

        GameMode m_GameMode{ GameMode::SinglePlayer };

        Scene* m_pGameScene{ nullptr };
        LevelLoader m_LevelLoader;
        GridComponent* m_pGrid{ nullptr };


        GameObject* m_pPlayer1{ nullptr };
        GameObject* m_pPlayer2{ nullptr };

        void SetupInputBindings();
        void CleanupInputBindings();
    };
}
