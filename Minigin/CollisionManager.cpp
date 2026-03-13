#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"

void dae::CollisionManager::AddCollider(CollisionComponent* pCollider)
{
	m_pColliders.push_back(pCollider);
}

void dae::CollisionManager::RemoveCollider(CollisionComponent* pCollider)
{
	m_pColliders.remove(pCollider);
}

dae::GameObject* dae::CollisionManager::CheckCollision(CollisionComponent* pCollider)
{
	const auto transform = pCollider->GetTransform();
	const auto position = transform->GetWorldPosition();
	const auto size = pCollider->GetSize();

	for (auto otherCollider : m_pColliders)
	{
		if (pCollider == otherCollider) continue;

		const auto otherTransform = otherCollider->GetTransform();
		const auto otherPosition = otherTransform->GetWorldPosition();
		const auto otherSize = otherCollider->GetSize();

		//AABB
		if (position.x < otherPosition.x + otherSize.x && position.x + size.x > otherPosition.x &&
			position.y < otherPosition.y + otherSize.y && position.y + size.y > otherPosition.y)
		{
			pCollider->OnCollision(otherCollider->GetGameObject());
			otherCollider->OnCollision(pCollider->GetGameObject());
		}
	}

	return nullptr;
}
