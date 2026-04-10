#include "SnoBeeComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Components/Blocks/IceBlockComponent.h"
#include <cstdlib>
#include "Events/EventManager.h"
#include "Observers/Subject.h"

dae::SnoBeeComponent::SnoBeeComponent(GameObject* gameObject, GridComponent* grid)
    : GameComponent(gameObject),
    m_pGrid{ grid },
    m_pSubject{ std::make_unique<Subject>(10) }
{
    m_pMove = gameObject->GetComponent<GridMoveComponent>();
    EventManager::GetInstance().AddEvent(make_sdbm_hash("StunEnemies"), this);
}

void dae::SnoBeeComponent::Update()
{
    const float dt = GameTime::GetInstance().GetDeltaTime();


    if (m_State == SnoBeeState::Stunned)
    {
        m_StunTimer -= dt;
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
        m_BlockBreakTimer -= dt;
    }

    if (m_pMove->IsMoving()) return;

    UpdateWander();
}

void dae::SnoBeeComponent::UpdateWander()
{
    if (m_BlockBreakTimer > 0.f) return;


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

    auto* block = occupant->GetComponent<IceBlockComponent>();
    if (!block) return false;

    block->OnBreak();

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