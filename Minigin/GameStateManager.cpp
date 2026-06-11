#include "GameStateManager.h"

void dae::GameStateManager::Update()
{
    if (m_pCurrentState)
    {
        auto nextState = m_pCurrentState->Update();

        if (nextState != nullptr)
        {
            ChangeState(std::move(nextState));
        }
    }
}

void dae::GameStateManager::ChangeState(std::unique_ptr<GameState> newState)
{
    if (m_pCurrentState)
    {
        m_pCurrentState->OnExit();
    }

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
    {
        m_pCurrentState->OnEnter();
    }
}