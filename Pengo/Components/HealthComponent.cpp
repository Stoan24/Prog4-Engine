#include "HealthComponent.h"

#include "SDBMHasher.h"
#include "Transform.h"
#include "GameTime.h"
#include "Events/EventManager.h"

#include "SnoBeeComponent.h"


dae::HealthComponent::HealthComponent(GameObject* gameObject, int lives)
	:GameComponent(gameObject),
	m_TotalLives{lives},
	m_pSubject{ std::make_unique<Subject>(10) }
{
	m_Lives = m_TotalLives;

	EventManager::GetInstance().AddEvent(make_sdbm_hash("OnCollision"), this);
}

dae::HealthComponent::~HealthComponent()
{
	EventManager::GetInstance().RemoveObserver(make_sdbm_hash("OnCollision"), this);
}

void dae::HealthComponent::Update()
{
	if (m_IsInvincible)
	{
		float deltaTime = GameTime::GetInstance().GetDeltaTime();
		m_InvincibleTimer -= deltaTime;


		if (m_InvincibleTimer <= 0.0f)
		{
			m_IsInvincible = false;
			m_InvincibleTimer = 0.0f;
		}
	}
}

void dae::HealthComponent::Notify(const Event& e)
{
	if (e.id == make_sdbm_hash("OnCollision"))
	{
		GameObject* objA = e.args[0].gameObject;
		GameObject* objB = e.args[1].gameObject;


		GameObject* other = nullptr;
		if (objA == GetGameObject())
		{
			other = objB;
		}
		else if (objB == GetGameObject())
		{
			other = objA;
		}

		if (!other) return;


		if (other->HasTag("Enemy"))
		{
			auto* snoBee = other->GetComponent<SnoBeeComponent>();
			if (snoBee)
			{
				if (!snoBee->IsStunned())
				{
					if (!m_IsInvincible && !m_IsDead)
					{
						TakeDamage(1);
					}
				}
			}
		}
	}
}

void dae::HealthComponent::TakeDamage(int amount)
{
	if (m_IsDead || m_IsInvincible) return;

	m_Lives -= amount;

	m_IsInvincible = true;
	m_InvincibleTimer = m_MaxInvincibleTime;

	m_pSubject->NotifyObservers(GetGameObject(), make_sdbm_hash("PlayerHit"));

	if (m_Lives <= 0)
	{
		m_Lives = 0;
		m_IsDead = true;
	}
	else
	{
		Respawn();
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

void dae::HealthComponent::Respawn()
{
	auto* gridMove = GetGameObject()->GetComponent<GridMoveComponent>();
	if (gridMove)
	{
		gridMove->ResetToCell(m_SpawnCell.x, m_SpawnCell.y);
	}
}
