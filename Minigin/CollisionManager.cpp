#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"
#include <vector>
#include <SDBMHasher.h>
#include "Events/Event.h"
#include "Events/EventManager.h"

void dae::CollisionManager::AddCollider(CollisionComponent* collider)
{
	m_pColliders.push_back(collider);
}

void dae::CollisionManager::RemoveCollider(CollisionComponent* collider)
{
	std::erase(m_pColliders, collider);
}

void dae::CollisionManager::FixedUpdate()
{
    for (size_t i = 0; i < m_pColliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_pColliders.size(); ++j)
        {
            auto* colliderA = m_pColliders[i];
            auto* colliderB = m_pColliders[j];

            const auto posA = colliderA->GetTransform()->GetWorldPosition();
            const auto sizeA = colliderA->GetSize();
            const auto posB = colliderB->GetTransform()->GetWorldPosition();
            const auto sizeB = colliderB->GetSize();

            //AABB Check
            if (posA.x < posB.x + sizeB.x && posA.x + sizeA.x > posB.x &&
                posA.y < posB.y + sizeB.y && posA.y + sizeA.y > posB.y)
            {
                Event e(make_sdbm_hash("OnCollision"));
                e.nbArgs = 2;
                e.args[0].gameObject = colliderA->GetGameObject();
                e.args[1].gameObject = colliderB->GetGameObject();

                EventManager::GetInstance().HandleEvent(e);
            }
        }
    }
}
