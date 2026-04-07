#include "IceBlockComponent.h"
#include "GameObject.h"
#include "Transform.h"

dae::IceBlockComponent::IceBlockComponent(GameObject* go, GridComponent* grid, int col, int row)
    : GameComponent(go),
    m_Grid{ grid },
    m_Cell{ col, row }
{
    glm::vec2 worldPos = m_Grid->CellToWorld(col, row);
    go->GetComponent<Transform>()->SetLocalPosition(worldPos.x, worldPos.y);


    m_Grid->FillCell(col, row, go);
}

dae::IceBlockComponent::~IceBlockComponent()
{
    //m_Grid->FreeCell(m_Cell.x, m_Cell.y);
}