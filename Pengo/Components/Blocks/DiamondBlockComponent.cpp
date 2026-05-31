#include "DiamondBlockComponent.h"
#include "GameObject.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"
#include "Sound/ServiceLocator.h"

dae::DiamondBlockComponent::DiamondBlockComponent(GameObject* gameObject, GridComponent* grid)
    : BlockComponent(gameObject, grid)
{
}

void dae::DiamondBlockComponent::CheckAlignment()
{
    if (!m_pGrid || !m_pMoveComponent) return;

    glm::ivec2 currentCell = m_pMoveComponent->GetCurrentCell();


    int horizontalCount = 1;

    //Left
    for (int c = currentCell.x - 1; c >= 0; --c)
    {
        GameObject* obj = m_pGrid->GetCellObject(c, currentCell.y);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            horizontalCount++;
        }
        else
        {
            break;
        }
    }

    //Right
    for (int c = currentCell.x + 1; c < m_pGrid->GetCols(); ++c)
    {
        GameObject* obj = m_pGrid->GetCellObject(c, currentCell.y);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            horizontalCount++;
        }
        else
        {
            break;
        }
    }


    int verticalCount = 1;

    //Up
    for (int r = currentCell.y - 1; r >= 0; --r)
    {
        GameObject* obj = m_pGrid->GetCellObject(currentCell.x, r);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            verticalCount++;
        }
        else
        {
            break;
        }
    }

    //Down
    for (int r = currentCell.y + 1; r < m_pGrid->GetRows(); ++r)
    {
        GameObject* obj = m_pGrid->GetCellObject(currentCell.x, r);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            verticalCount++;
        }
        else
        {
            break;
        }
    }

    //If aligned, stun enemies
    if (horizontalCount >= 3 || verticalCount >= 3)
    {
        Event e(make_sdbm_hash("StunEnemies"));
        e.nbArgs = 0;
        e.args[0].gameObject = m_pPlayer;
        EventManager::GetInstance().HandleEvent(e);

        ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeStunned"), 0.05f);
    }
}