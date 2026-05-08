#pragma once
#include <memory>
#include <string>

namespace dae
{
    class GameStateManager;

    class GameState
    {
    public:
        virtual ~GameState() = default;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

        virtual std::unique_ptr<GameState> Update() = 0;

        virtual void Render() = 0;

    protected:
        GameStateManager* m_pStateManager{ nullptr };

        friend class GameStateManager;
        void SetStateManager(GameStateManager* manager) { m_pStateManager = manager; }
    };
}