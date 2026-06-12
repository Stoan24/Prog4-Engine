#pragma once
#include "GameState.h"
#include <memory>

namespace dae
{
    enum class GameMode
    {
        SinglePlayer,
        Coop,
        Versus
    };

    class Scene;

    class StartMenuState final : public GameState
    {
    public:
        StartMenuState() = default;
        ~StartMenuState() override = default;

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<GameState> Update() override;

    private:
        Scene* m_pMenuScene{ nullptr };

        void CreateMenuScene();
        void SetupInputBindings();
        void CleanupInputBindings();
    };
}
