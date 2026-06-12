#include "PlayerManager.h"

#include "Components/ScoreComponent.h"


void dae::PlayerManager::Initialize(int playerCount)
{
    if (!m_Players.empty()) return;

    m_Players.resize(playerCount);
    for (int i = 0; i < playerCount; ++i)
    {
        m_Players[i].playerIdx = i;
    }
}

dae::GameObject* dae::PlayerManager::GetPlayerObject(int playerIdx) const
{
    if (playerIdx < 0 || playerIdx >= static_cast<int>(m_PlayerObjects.size())) return nullptr;
    return m_PlayerObjects[playerIdx];
}

void dae::PlayerManager::RegisterPlayer(int playerIdx, GameObject* playerObject)
{
    if (playerIdx >= static_cast<int>(m_PlayerObjects.size()))
    {
        m_PlayerObjects.resize(playerIdx + 1, nullptr);
    }

    m_PlayerObjects[playerIdx] = playerObject;
}

int dae::PlayerManager::GetPlayerIndex(GameObject* playerObject) const
{
    for (int i = 0; i < static_cast<int>(m_PlayerObjects.size()); ++i)
    {
        if (m_PlayerObjects[i] == playerObject) return i;
    }

    return -1;
}
