#include "BlockComponent.h"

#include "Events/Event.h"
#include "Events/EventManager.h"
#include "ServiceLocator.h"
#include "CollisionManager.h"
#include "SDBMHasher.h"

dae::BlockComponent::BlockComponent(GameObject* gameObject, GridComponent* grid)
    :GameComponent(gameObject),
    m_pGrid{ grid }
{
    m_pMoveComponent = gameObject->GetComponent<GridMoveComponent>();
    m_pCollisionComponent = gameObject->GetComponent<CollisionComponent>();

    m_pGrid->FillCell(m_pMoveComponent->GetCurrentCell().x, m_pMoveComponent->GetCurrentCell().y, gameObject);

    EventManager::GetInstance().AddEvent(make_sdbm_hash("OnCollision"), this);
}

dae::BlockComponent::~BlockComponent()
{
    EventManager::GetInstance().RemoveObserver(make_sdbm_hash("OnCollision"), this);
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
                Event e(make_sdbm_hash("EnemyKilled"));
                e.nbArgs = 1;
                e.args[0].gameObject = m_pPlayer;
                e.args[0].value = m_KillCount;
                EventManager::GetInstance().HandleEvent(e);

                m_KillCount = 0;
            }

            OnSlideStopped();
            m_pPlayer = nullptr;
        }
    }
}

void dae::BlockComponent::HandleCollisionWith(GameObject* other)
{
    if (!m_IsSliding) return;

    if (other->HasTag("Enemy") && !other->IsMarkedForDestruction())
    {
        ++m_KillCount;
        ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeSquashed"), 0.05f);
        other->MarkForDestruction();
    }
}

void dae::BlockComponent::Notify(const Event& e)
{
    if (e.id == make_sdbm_hash("OnCollision"))
    {
        GameObject* objA = e.args[0].gameObject;
        GameObject* objB = e.args[1].gameObject;

        if (objA == GetGameObject())
        {
            HandleCollisionWith(objB);
        }
        else if (objB == GetGameObject())
        {
            HandleCollisionWith(objA);
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
