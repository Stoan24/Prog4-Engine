#include "IceBlockComponent.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include <SDBMHasher.h>
#include <Events/Event.h>
#include <Events/EventManager.h>

dae::IceBlockComponent::IceBlockComponent(GameObject* gameObject, GridComponent* grid)
    : BlockComponent(gameObject, grid)
{
}

void dae::IceBlockComponent::OnBreak(bool byPlayer)
{
    if (m_pGrid && m_pMoveComponent)
    {
        glm::ivec2 currentCell = m_pMoveComponent->GetCurrentCell();
        m_pGrid->FreeCell(currentCell.x, currentCell.y);


        if (byPlayer)
        {
            Event e(make_sdbm_hash("IceBlockDestroyed"));
            e.nbArgs = 1;
            e.args[0].gameObject = m_pPlayer;
            EventManager::GetInstance().HandleEvent(e);

            ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("IceBlockDestroyed"), 0.05f);
        }
    }

    GetGameObject()->MarkForDestruction();
}
