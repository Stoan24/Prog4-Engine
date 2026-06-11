#include "BlockComponent.h"

#include "Events/Event.h"
#include "Events/EventManager.h"
#include "Sound/ServiceLocator.h"
#include "CollisionManager.h"
#include "SDBMHasher.h"

dae::BlockComponent::BlockComponent(GameObject* gameObject, GridComponent* grid)
    :GameComponent(gameObject),
    m_pGrid{ grid }
{
    m_pMoveComponent = gameObject->GetComponent<GridMoveComponent>();
    m_pCollisionComponent = gameObject->GetComponent<CollisionComponent>();

    m_pGrid->FillCell(m_pMoveComponent->GetCurrentCell().x, m_pMoveComponent->GetCurrentCell().y, gameObject);
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


            if (m_KillCount > 0 && m_pPlayer)
            {
                int count = std::min(m_KillCount, m_MaxKills);
                int totalScore = m_KillScores[count];

                Event e(make_sdbm_hash("EnemyKilled"));
                e.nbArgs = 1;
                e.args[0].gameObject = m_pPlayer;
                e.args[0].score = totalScore;
                EventManager::GetInstance().HandleEvent(e);

                m_KillCount = 0;
            }

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

            ++m_KillCount;


            ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeSquashed"), 0.05f);


            hitObject->MarkForDestruction();
        }
    }
}

void dae::BlockComponent::Push(glm::ivec2 direction, GameObject* player)
{
    if (m_IsSliding) return;

    m_SlideDirection = direction;

    m_pPlayer = player;

    if (m_pMoveComponent->Move(m_SlideDirection, true))
    {
        ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("PushIceBlock"), 0.05f);
        m_IsSliding = true;
    }
    else
    {
        OnBreak(true);
    }
}
