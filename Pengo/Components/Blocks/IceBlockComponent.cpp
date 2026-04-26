#include "IceBlockComponent.h"
#include "GameObject.h"

dae::IceBlockComponent::IceBlockComponent(GameObject* gameObject, GridComponent* grid)
    : BlockComponent(gameObject, grid)
{
}

void dae::IceBlockComponent::OnBreak()
{
    if (m_pGrid && m_pMoveComponent)
    {
        glm::ivec2 currentCell = m_pMoveComponent->GetCurrentCell();
        m_pGrid->FreeCell(currentCell.x, currentCell.y);
    }

    GetGameObject()->MarkForDestruction();
}
