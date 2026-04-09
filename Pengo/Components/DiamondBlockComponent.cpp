#include "DiamondBlockComponent.h"
#include "GameObject.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"

dae::DiamondBlockComponent::DiamondBlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row)
    : BlockComponent(gameObject, grid, col, row)
{
}

void dae::DiamondBlockComponent::CheckAlignment()
{
    if (!m_pGrid || !m_pMoveComponent) return;

    glm::ivec2 currentCell = m_pMoveComponent->GetCurrentCell();

    //Check Row
    int blocksInRow = 0;
    for (int c = 0; c < m_pGrid->GetCols(); ++c)
    {
        GameObject* obj = m_pGrid->GetCellObject(c, currentCell.y);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            blocksInRow++;
        }
    }

    //Check Column
    int blocksInCol = 0;
    for (int r = 0; r < m_pGrid->GetRows(); ++r)
    {
        GameObject* obj = m_pGrid->GetCellObject(currentCell.x, r);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            blocksInCol++;
        }
    }

    //If aligned, stun enemies
    if (blocksInRow >= 3 || blocksInCol >= 3)
    {
        Event e(make_sdbm_hash("StunEnemies"));
        e.nbArgs = 0;
        e.args[0].gameObject = m_pPlayer;
        EventManager::GetInstance().HandleEvent(e);
    }
}