#pragma once
#include "Singleton.h"
#include <vector>
#include "Components/CollisionComponent.h"

namespace dae
{
	class GameObject;
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:

		void AddCollider(CollisionComponent* collider);
		void RemoveCollider(CollisionComponent* collider);

		GameObject* CheckCollision(CollisionComponent* collider);

	private:
		//Friend class, can only be made by itself
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;

		std::vector<CollisionComponent*> m_pColliders{};
	};
}