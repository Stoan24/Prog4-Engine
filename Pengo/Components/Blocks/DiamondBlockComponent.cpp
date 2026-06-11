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

    const int maxCols = m_pGrid->GetCols();
    const int maxRows = m_pGrid->GetRows();

    //Touching a wall?
    bool horizontalTouchesEdge = (currentCell.x == 0 || currentCell.x == maxCols - 1);
    bool verticalTouchesEdge = (currentCell.y == 0 || currentCell.y == maxRows - 1);

    int horizontalCount = 1;

    //Left
    for (int c = currentCell.x - 1; c >= 0; --c)
    {
        GameObject* obj = m_pGrid->GetCellObject(c, currentCell.y);
        if (obj && obj->GetComponent<DiamondBlockComponent>())
        {
            horizontalCount++;
            if (c == 0) horizontalTouchesEdge = true;
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
            if (c == maxCols - 1) horizontalTouchesEdge = true;
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
            if (r == 0) verticalTouchesEdge = true;
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
            if (r == maxRows - 1) verticalTouchesEdge = true;
        }
        else
        {
            break;
        }
    }

    bool isHorizontalMatch = (horizontalCount >= 3);
    bool isVerticalMatch = (verticalCount >= 3);


    //If aligned, stun enemies
    if (isHorizontalMatch || isVerticalMatch)
    {
        bool alignmentTouchesEdge = false;
        if (isHorizontalMatch && horizontalTouchesEdge) alignmentTouchesEdge = true;
        if (isVerticalMatch && verticalTouchesEdge) alignmentTouchesEdge = true;

        Event e(make_sdbm_hash("StunEnemies"));
        e.nbArgs = 1;
        e.args[0].gameObject = m_pPlayer;

        if (alignmentTouchesEdge)
        {
            e.args[1].score = m_stunSideScore;
        }
        else
        {
            e.args[1].score = m_stunMiddleScore;
        }

        EventManager::GetInstance().HandleEvent(e);

        ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeStunned"), 0.05f);
    }
}