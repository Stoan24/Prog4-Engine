#include "SnoBeeComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Components/Blocks/IceBlockComponent.h"
#include <cstdlib>
#include "Events/EventManager.h"
#include "Blocks/EggBlockComponent.h"

dae::SnoBeeComponent::SnoBeeComponent(GameObject* gameObject, GridComponent* grid, bool isPlayerControlled)
    : GameComponent(gameObject),
    m_pGrid{ grid },
    m_IsPlayerControlled{ isPlayerControlled }
{
    m_pMove = gameObject->GetComponent<GridMoveComponent>();
    EventManager::GetInstance().AddEvent(make_sdbm_hash("StunEnemies"), this);
    EventManager::GetInstance().AddEvent(make_sdbm_hash("OnCollision"), this);
}

dae::SnoBeeComponent::~SnoBeeComponent()
{
    EventManager::GetInstance().RemoveObserver(make_sdbm_hash("OnCollision"), this);
}

void dae::SnoBeeComponent::FixedUpdate()
{
    const float deltaTime = GameTime::GetInstance().GetFixedDeltaTime();


    if (m_State == SnoBeeState::Stunned)
    {
        m_StunTimer -= deltaTime;
        if (m_StunTimer <= 0.f)
        {
            m_State = SnoBeeState::Wander;
            m_WanderTarget = { -1, -1 };
        }
        return;
    }


    if (!m_pMove) return;


    if (m_BlockBreakTimer > 0.f)
    {
        m_BlockBreakTimer -= deltaTime;
        return;
    }

    if (m_IsPlayerControlled) return;

    if (m_pMove->IsMoving()) return;

    UpdateWander();
}

void dae::SnoBeeComponent::UpdateWander()
{
    glm::ivec2 currentCell = m_pMove->GetCurrentCell();
    
    if (m_WanderTarget == glm::ivec2{ -1, -1 } || currentCell == m_WanderTarget)
    {
        m_WanderTarget = PickRandomCell();
    }

    if (m_WanderTarget == currentCell) return;


    glm::ivec2 direction = m_WanderTarget - currentCell;
    

    auto* occupant = m_pGrid->GetCellObject(m_WanderTarget.x, m_WanderTarget.y);
    if (occupant && occupant->GetComponent<IceBlockComponent>())
    {
        if (BreakBlock(direction))
        {
            m_WanderTarget = { -1, -1 };
        }
        return;
    }


    if (!m_pMove->Move(direction, false))
    {
        m_WanderTarget = { -1, -1 };
    }
}

bool dae::SnoBeeComponent::BreakBlock(glm::ivec2 direction)
{
    if (m_BlockBreakTimer > 0.f) return false;

    glm::ivec2 targetCell = m_pMove->GetCurrentCell() + direction;
    auto* occupant = m_pGrid->GetCellObject(targetCell.x, targetCell.y);

    if (!occupant) return false;

    if (occupant->GetComponent<EggBlockComponent>()) return false;

    auto* block = occupant->GetComponent<IceBlockComponent>();
    if (!block) return false;

    block->OnBreak(false);

    m_BlockBreakTimer = m_BlockBreakCooldown;
    return true;
}

glm::ivec2 dae::SnoBeeComponent::PickRandomCell() const
{
    const glm::ivec2 directions[] = { {1,0},{-1,0},{0,1},{0,-1} };
    glm::ivec2 current = m_pMove->GetCurrentCell();

    std::vector<glm::ivec2> potentialCells;

    //For each direction look what is inside that cell
    for (auto dir : directions)
    {
        glm::ivec2 next = current + dir;

        if (m_pGrid->IsValidCell(next))
        {
            //Empty Cell
            if (m_pGrid->IsCellFree(next.x, next.y))
            {
                potentialCells.push_back(next);
            }
            //Breakable Cell
            else if (auto* occupant = m_pGrid->GetCellObject(next.x, next.y))
            {
                if (occupant->GetComponent<IceBlockComponent>())
                {
                    potentialCells.push_back(next);
                }
            }
        }
    }

    if (potentialCells.empty()) return current;

    //Choose a random cell out of the options
    return potentialCells[rand() % static_cast<int>(potentialCells.size())];
}

void dae::SnoBeeComponent::Stun(float duration)
{
    m_State = SnoBeeState::Stunned;
    m_StunTimer = duration;
    m_WanderTarget = { -1, -1 };
}

void dae::SnoBeeComponent::Notify(const Event& e)
{
    if (e.id == make_sdbm_hash("StunEnemies"))
    {
        Stun(m_maxStun);
    }
    else if (e.id == make_sdbm_hash("OnCollision"))
    {
        GameObject* objA = e.args[0].gameObject;
        GameObject* objB = e.args[1].gameObject;

        GameObject* other = nullptr;
        if (objA == GetGameObject()) other = objB;
        else if (objB == GetGameObject()) other = objA;


        if (!other) return;

        //Kill Snobee when stunned
        if (other->HasTag("Player") && IsStunned())
        {
            if (GetGameObject()->IsMarkedForDestruction()) return;

            Event scoreEvent(make_sdbm_hash("EnemyKilled"));
            scoreEvent.nbArgs = 1;
            scoreEvent.args[0].gameObject = other;
            EventManager::GetInstance().HandleEvent(scoreEvent);

            GetGameObject()->MarkForDestruction();
        }
    }
}

bool dae::SnoBeeComponent::TryManualBlockBreak(glm::ivec2 direction)
{
    return BreakBlock(direction);
}