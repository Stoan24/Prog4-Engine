#pragma once
#include "GameState.h"
#include <memory>

namespace dae
{
    class Scene;

    class StartMenuState final : public GameState
    {
    public:
        StartMenuState() = default;
        ~StartMenuState() override = default;

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<dae::GameState> Update() override;
        void Render() override;


        void RequestStateChange(std::unique_ptr<GameState> nextState) { m_pPendingState = std::move(nextState); }


    private:
        std::unique_ptr<GameState> m_pPendingState{ nullptr };
        Scene* m_pMenuScene{ nullptr };

        void SetupInputBindings();
        void CleanupInputBindings();
        void CreateMenuScene();
    };
}
