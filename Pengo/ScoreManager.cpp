#include "ScoreManager.h"
#include "Components/ScoreComponent.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"
#include <vector>

void dae::ScoreManager::Initialize()
{
	EventManager::GetInstance().AddEvent(make_sdbm_hash("EnemyKilled"), this);
	EventManager::GetInstance().AddEvent(make_sdbm_hash("StunEnemies"), this);
}

void dae::ScoreManager::Notify(const Event& e)
{
	auto* score = e.args[0].gameObject->GetComponent<ScoreComponent>();
	if (!score) return;

	if (e.id == make_sdbm_hash("EnemyKilled"))  score->AddPoints(m_killScore);
	if (e.id == make_sdbm_hash("StunEnemies"))  score->AddPoints(m_stunScore);
}
