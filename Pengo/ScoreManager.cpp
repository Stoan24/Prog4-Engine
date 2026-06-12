#include "ScoreManager.h"
#include "Events/EventManager.h"
#include "PlayerManager.h"
#include "Components/ScoreComponent.h"

#include "SDBMHasher.h"
#include <algorithm>

void dae::ScoreManager::Initialize()
{
    EventManager::GetInstance().AddEvent(make_sdbm_hash("EnemyKilled"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("StunEnemies"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("EggDestroyed"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("IceBlockDestroyed"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("LevelFinish"), this);
}

void dae::ScoreManager::Notify(const Event& e)
{
    if (e.nbArgs < 1 || !e.args[0].gameObject) return;

    int playerIdx = PlayerManager::GetInstance().GetPlayerIndex(e.args[0].gameObject);
    if (playerIdx == -1) return;

    int points = CalculatePoints(e);
    if (points != 0) AddScore(playerIdx, points);
}

int dae::ScoreManager::GetScore(int playerIdx) const
{
    return m_scores[playerIdx];
}

void dae::ScoreManager::SetScore(int playerIdx, int score)
{
    m_scores[playerIdx] = score;
}

void dae::ScoreManager::AddScore(int playerIdx, int amount)
{
    m_scores[playerIdx] += amount;

    auto* obj = PlayerManager::GetInstance().GetPlayerObject(playerIdx);
    if (!obj) return;

    auto* scoreComp = obj->GetComponent<ScoreComponent>();
    if (scoreComp) scoreComp->AddPoints(amount);
}

int dae::ScoreManager::CalculatePoints(const Event& e) const
{
    if (e.id == make_sdbm_hash("EnemyKilled"))
    {
        int killCount = std::clamp(e.args[0].value, 0, m_MaxKills);
        return m_KillScores[killCount];
    }

    if (e.id == make_sdbm_hash("StunEnemies"))
    {
        return e.args[0].value != 0 ? m_StunSideScore : m_StunMiddleScore;
    }

    if (e.id == make_sdbm_hash("EggDestroyed"))
    {
        return m_EggBreakScore;
    }

    if (e.id == make_sdbm_hash("IceBlockDestroyed"))
    {
        return m_IceBlockScore;
    }

    if (e.id == make_sdbm_hash("LevelFinish"))
    {
        return CalculateTimeBonus(e.args[0].value);
    }

    return 0;
}

int dae::ScoreManager::CalculateTimeBonus(int seconds) const
{
    if (seconds >= 1 && seconds <= 20)   return 5000;
    if (seconds >= 21 && seconds <= 29)  return 2000;
    if (seconds >= 30 && seconds <= 39)  return 1000;
    if (seconds >= 40 && seconds <= 49)  return 500;
    if (seconds >= 50 && seconds <= 59)  return 10;

    return 0;
}
