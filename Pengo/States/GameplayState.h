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
        GameplayState() = default;
        ~GameplayState() override = default;

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<dae::GameState> Update() override;
        void Render() override;

        void RequestStateChange(std::unique_ptr<GameState> nextState) { m_pPendingState = std::move(nextState); }


    private:
        std::unique_ptr<GameState> m_pPendingState{ nullptr };

        Scene* m_pGameScene{ nullptr };
        LevelLoader m_LevelLoader;
        GridComponent* m_pGrid{ nullptr };


        GameObject* m_pPlayer1{ nullptr };
        GameObject* m_pPlayer2{ nullptr };

        void SetupInputBindings();
        void CleanupInputBindings();
        void LoadLevel();
    };
}
