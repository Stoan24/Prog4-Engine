#include "HealthComponent.h"
#include <SDBMHasher.h>
#include "Events/EventManager.h"

#include "Transform.h"


dae::HealthComponent::HealthComponent(GameObject* gameObject, int lives)
	:GameComponent(gameObject),
	m_TotalLives{lives},
	m_pSubject{ std::make_unique<Subject>(10) }
{
	EventManager::GetInstance().AddEvent(make_sdbm_hash("PlayerHit"), this);

	m_Lives = m_TotalLives;
}

void dae::HealthComponent::Notify(const Event& e)
{
	if (e.id == make_sdbm_hash("PlayerHit"))
	{
		if (e.args[0].gameObject == GetGameObject())
		{
			TakeDamage(1);
		}
	}
}

void dae::HealthComponent::TakeDamage(int amount)
{
	if (m_IsDead) return;

	m_Lives -= amount;

	//Quick Teleport instead of full respawn function
	auto transform = GetGameObject()->GetComponent<Transform>();
	if (transform)
	{
		transform->SetLocalPosition(300, 350);
	}

	m_pSubject->NotifyObservers(GetGameObject(), make_sdbm_hash("PlayerHit"));

	if (m_Lives <= 0)
	{
		m_IsDead = true;
	}
}

void dae::HealthComponent::RestoreLives()
{
	m_Lives = m_TotalLives;
}

bool dae::HealthComponent::IsDead()
{
	return m_IsDead;
}
