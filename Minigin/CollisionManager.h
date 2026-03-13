#pragma once
#include "Singleton.h"
#include <list>
#include "Components/CollisionComponent.h"

namespace dae
{
	class GameObject;
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		CollisionManager() = default;

		void AddCollider(CollisionComponent* pCollider);
		void RemoveCollider(CollisionComponent* pCollider);

		GameObject* CheckCollision(CollisionComponent* pCollider);

	private:

		std::list<CollisionComponent*> m_pColliders{};
	};
}