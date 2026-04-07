#include "IceBlockComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "CollisionManager.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"

dae::IceBlockComponent::IceBlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row)
    : GameComponent(gameObject),
    m_pGrid{ grid },
    m_Cell{ col, row }
{
    //glm::vec2 worldPos = m_Grid->CellToWorld(col, row);
    //gameObject->GetComponent<Transform>()->SetLocalPosition(worldPos.x, worldPos.y);

    m_pGrid->FillCell(col, row, gameObject);

    m_pMoveComponent = gameObject->GetComponent<GridMoveComponent>();
    m_pCollisionComponent = gameObject->GetComponent<CollisionComponent>();

    
}

dae::IceBlockComponent::~IceBlockComponent()
{
    //m_Grid->FreeCell(m_Cell.x, m_Cell.y);
}

void dae::IceBlockComponent::Update()
{
    if (!m_IsSliding) return;

    //movement
    if (m_pMoveComponent && !m_pMoveComponent->IsMoving())
    {
        if (!m_pMoveComponent->Move(m_SlideDirection, true))
        {
            m_IsSliding = false;
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

void dae::IceBlockComponent::Push(glm::ivec2 direction, GameObject* player)
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
        Break();
    }
}

void dae::IceBlockComponent::Break()
{
    if (m_pGrid && m_pMoveComponent)
    {
        glm::ivec2 currentCell = m_pMoveComponent->GetCurrentCell();
        m_pGrid->FreeCell(currentCell.x, currentCell.y);
    }

    //Handle Events here?

    GetGameObject()->MarkForDestruction();
}
