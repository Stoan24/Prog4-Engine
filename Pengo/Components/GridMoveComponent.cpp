#include "GridMoveComponent.h"
#include "Transform.h"
#include "GameTime.h"

dae::GridMoveComponent::GridMoveComponent(GameObject* gameObject, GridComponent* grid, int startCol, int startRow, float moveSpeed)
    : GameComponent(gameObject),
    m_pGrid{ grid },
    m_CurrentCell{ startCol, startRow },
    m_TargetCell{ startCol, startRow },
    m_MoveSpeed{ moveSpeed }
{
    glm::vec2 startPos = m_pGrid->CellToWorld(startCol, startRow);
    gameObject->GetComponent<Transform>()->SetLocalPosition(startPos.x, startPos.y);

    m_StartWorldPos = startPos;
}

void dae::GridMoveComponent::Update()
{
    if (!m_IsMoving) return;

    const float deltaTime = GameTime::GetInstance().GetDeltaTime();
    m_MoveProgress += deltaTime * m_MoveSpeed;

    glm::vec2 targetWorldPos = m_pGrid->CellToWorld(m_TargetCell);

    if (m_MoveProgress >= 1.f)
    {
        m_MoveProgress = 1.f;
        m_CurrentCell = m_TargetCell;
        m_IsMoving = false;

        GetGameObject()->GetComponent<Transform>()->SetLocalPosition(targetWorldPos.x, targetWorldPos.y);
    }
    else
    {
        //Lerp
        glm::vec2 pos = m_StartWorldPos + (targetWorldPos - m_StartWorldPos) * m_MoveProgress;
        GetGameObject()->GetComponent<Transform>()->SetLocalPosition(pos.x, pos.y);
    }
}

bool dae::GridMoveComponent::Move(glm::ivec2 direction, bool isBlock)
{
    if (m_IsMoving) return false;

    m_LookDirection = direction;

    glm::ivec2 newCell = m_CurrentCell + direction;

    if (!m_pGrid->IsValidCell(newCell)) return false;
    if (!m_pGrid->IsCellFree(newCell.x, newCell.y)) return false;

    if (isBlock)
    {
        m_pGrid->FreeCell(m_CurrentCell.x, m_CurrentCell.y);
        m_pGrid->FillCell(newCell.x, newCell.y, GetGameObject());
    }

    m_TargetCell = newCell;
    m_StartWorldPos = m_pGrid->CellToWorld(m_CurrentCell);
    m_MoveProgress = 0.f;
    m_IsMoving = true;

    return true;
}

glm::ivec2 dae::GridMoveComponent::GetLookDirection()
{
    return m_LookDirection;
}
