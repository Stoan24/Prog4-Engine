#include "BlockComponent.h"
#include "CollisionManager.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"

dae::BlockComponent::BlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row)
    :GameComponent(gameObject),
    m_pGrid{ grid }
{
    m_pGrid->FillCell(col, row, gameObject);

    m_pMoveComponent = gameObject->GetComponent<GridMoveComponent>();
    m_pCollisionComponent = gameObject->GetComponent<CollisionComponent>();
}

void dae::BlockComponent::Update()
{
    if (!m_IsSliding) return;

    //Movement
    if (m_pMoveComponent && !m_pMoveComponent->IsMoving())
    {
        if (!m_pMoveComponent->Move(m_SlideDirection, true))
        {
            m_IsSliding = false;

            OnSlideStopped();
            m_pPlayer = nullptr;
        }
    }

    //Kill enemy on collision
    auto* myCollider = GetGameObject()->GetComponent<CollisionComponent>();
    if (myCollider)
    {
        GameObject* hitObject = CollisionManager::GetInstance().CheckCollision(myCollider);

        if (hitObject && hitObject->HasTag("Enemy"))
        {
            if (hitObject->IsMarkedForDestruction()) return;

            Event e(make_sdbm_hash("EnemyKilled"));
            e.nbArgs = 1;
            e.args[0].gameObject = m_pPlayer;
            EventManager::GetInstance().HandleEvent(e);


            hitObject->MarkForDestruction();
        }
    }
}

void dae::BlockComponent::Push(glm::ivec2 direction, GameObject* player)
{
    if (m_IsSliding) return;

    m_SlideDirection = direction;

    if (m_pMoveComponent->Move(m_SlideDirection, true))
    {
        m_IsSliding = true;
        m_pPlayer = player;
    }
    else
    {
        OnBreak();
    }
}
