#include "ScoreComponent.h"
#include "ScoreManager.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"

dae::ScoreComponent::ScoreComponent(GameObject* gameObject)
	:GameComponent(gameObject)
{
	ScoreManager::GetInstance().AddScoreToTrack(this);
}

dae::ScoreComponent::~ScoreComponent()
{
	ScoreManager::GetInstance().RemoveScoreToTrack(this);
}

void dae::ScoreComponent::AddPoints(int amount)
{
	m_CurrentScore += amount;

	Event e(make_sdbm_hash("ScoreChange"));
	e.nbArgs = 2;
	e.args[0].gameObject = GetGameObject();
	e.args[1].value = m_CurrentScore;

	EventManager::GetInstance().HandleEvent(e);

	if (m_CurrentScore >= m_WinnerScore)
	{
		Event winner(make_sdbm_hash("Winner"));

		EventManager::GetInstance().HandleEvent(winner);
	}
}
