#include "ScoreComponent.h"
#include "ScoreManager.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"

dae::ScoreComponent::ScoreComponent(GameObject* gameObject)
	: GameComponent(gameObject)
	, m_pSubject{ std::make_unique<Subject>(10) }
{
}

void dae::ScoreComponent::AddPoints(int amount)
{
	m_CurrentScore += amount;

	m_pSubject->NotifyObservers(GetGameObject(), make_sdbm_hash("ScoreChanged"));
}
