#include "IceBlockComponent.h"
#include "GameObject.h"

dae::IceBlockComponent::IceBlockComponent(GameObject* gameObject, GridComponent* grid)
    : BlockComponent(gameObject, grid)
{
}

dae::IceBlockComponent::~IceBlockComponent()
{
    //m_Grid->FreeCell(m_Cell.x, m_Cell.y);
}

void dae::IceBlockComponent::OnBreak()
{
    if (m_pGrid && m_pMoveComponent)
    {
        glm::ivec2 currentCell = m_pMoveComponent->GetCurrentCell();
        m_pGrid->FreeCell(currentCell.x, currentCell.y);
    }

    //Handle Events here?

    GetGameObject()->MarkForDestruction();
}
