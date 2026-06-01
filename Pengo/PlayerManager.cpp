#include "Components/ScoreComponent.h"
#include "Events/EventManager.h"
#include "PlayerManager.h"
#include <SDBMHasher.h>
#include <Events/Event.h>

void dae::PlayerManager::Initialize(int playerCount)
{
    if (!m_Players.empty()) return;

    m_Players.resize(playerCount);
    for (int i = 0; i < playerCount; ++i)
    {
        m_Players[i].playerIdx = i;
    }
}

void dae::PlayerManager::OnLevelStart()
{
    EventManager::GetInstance().AddEvent(make_sdbm_hash("EnemyKilled"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("StunEnemies"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("EggDestroyed"), this);
}

void dae::PlayerManager::Notify(const Event& e)
{
    if (e.nbArgs < 1 || !e.args[0].gameObject) return;

    int playerIdx = GetPlayerIndex(e.args[0].gameObject);
    if (playerIdx == -1) return;

    if (e.id == make_sdbm_hash("EnemyKilled")) AddScore(playerIdx, m_killScore);
    if (e.id == make_sdbm_hash("StunEnemies")) AddScore(playerIdx, m_stunScore);
    if (e.id == make_sdbm_hash("EggDestroyed")) AddScore(playerIdx, m_eggScore);
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

void dae::PlayerManager::AddScore(int playerIdx, int score)
{
    m_Players[playerIdx].score += score;

    if (playerIdx < static_cast<int>(m_PlayerObjects.size()) && m_PlayerObjects[playerIdx])
    {
        auto* scoreComp = m_PlayerObjects[playerIdx]->GetComponent<ScoreComponent>();
        if (scoreComp) scoreComp->AddPoints(score);
    }
}
