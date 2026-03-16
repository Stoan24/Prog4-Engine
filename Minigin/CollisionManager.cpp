#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"
#include <vector>

void dae::CollisionManager::AddCollider(CollisionComponent* collider)
{
	m_pColliders.push_back(collider);
}

void dae::CollisionManager::RemoveCollider(CollisionComponent* collider)
{
	std::erase(m_pColliders, collider);
}

dae::GameObject* dae::CollisionManager::CheckCollision(CollisionComponent* collider)
{
	const auto transform = collider->GetTransform();
	const auto position = transform->GetWorldPosition();
	const auto size = collider->GetSize();

	for (auto otherCollider : m_pColliders)
	{
		if (collider == otherCollider) continue;

		const auto otherTransform = otherCollider->GetTransform();
		const auto otherPosition = otherTransform->GetWorldPosition();
		const auto otherSize = otherCollider->GetSize();

		//AABB
		if (position.x < otherPosition.x + otherSize.x && position.x + size.x > otherPosition.x &&
			position.y < otherPosition.y + otherSize.y && position.y + size.y > otherPosition.y)
		{
			collider->OnCollision(otherCollider->GetGameObject());
			otherCollider->OnCollision(collider->GetGameObject());
		}
	}

	return nullptr;
}
