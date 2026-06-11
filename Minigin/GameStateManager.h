#pragma once
#include <memory>
#include "GameState.h"
#include "Singleton.h"

namespace dae
{
    class GameStateManager final : public Singleton<GameStateManager>
    {
    public:

        void Update();

        void ChangeState(std::unique_ptr<GameState> newState);


    private:
        friend class Singleton<GameStateManager>;
        GameStateManager() = default;
        ~GameStateManager() = default;

        std::unique_ptr<GameState> m_pCurrentState{ nullptr };
    };
}
