#include "EggBlockComponent.h"
#include "Events/EventManager.h"
#include "Events/Event.h"
#include "SDBMHasher.h"
#include "GameObject.h"
#include "Components/GridComponent.h"
#include "Sound/ServiceLocator.h"

dae::EggBlockComponent::EggBlockComponent(GameObject* gameObject, GridComponent* grid)
    : BlockComponent(gameObject, grid)
{
}

void dae::EggBlockComponent::Hatch()
{
    m_HasHatched = true;

    ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeSpawning"), 0.05f);

    Destroy();
}

void dae::EggBlockComponent::OnBreak()
{
    if (m_HasHatched) return;

    ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeEggDestroyed"), 0.05f);


    Event e(make_sdbm_hash("EggDestroyed"));
    e.nbArgs = 1;
    e.args[0].gameObject = m_pPlayer;
    EventManager::GetInstance().HandleEvent(e);


    Destroy();
}

void dae::EggBlockComponent::Destroy()
{
    if (m_pGrid && m_pMoveComponent)
    {
        m_pGrid->FreeCell(m_pMoveComponent->GetCurrentCell().x, m_pMoveComponent->GetCurrentCell().y);
    }

    GetGameObject()->MarkForDestruction();
}