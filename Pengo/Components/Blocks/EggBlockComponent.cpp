#include "EggBlockComponent.h"
#include "Events/EventManager.h"
#include "Events/Event.h"
#include "SDBMHasher.h"
#include "GameObject.h"
#include "Components/GridComponent.h"
#include "ServiceLocator.h"
#include <GameTime.h>
#include <Components/TextureComponent.h>

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

void dae::EggBlockComponent::SetFlashing(bool flashing)
{
    m_IsFlashing = flashing;
    if (!flashing)
    {
        GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_NormalTexture);
    }
}

void dae::EggBlockComponent::UpdateFlash()
{
    if (!m_IsFlashing) return;

    m_FlashTimer += GameTime::GetInstance().GetDeltaTime();
    if (m_FlashTimer >= m_FlashInterval)
    {
        m_FlashTimer = 0.f;
        m_FlashToggle = !m_FlashToggle;
        GetGameObject()->GetComponent<TextureComponent>()->SetTexture(
            m_FlashToggle ? m_FlashTexture : m_NormalTexture
        );
    }
}

void dae::EggBlockComponent::OnBreak(bool playsSound)
{
    if (m_HasHatched) return;

    if (playsSound)
    {
        ServiceLocator::GetSoundSystem().Play(make_sdbm_hash("SnoBeeEggDestroyed"), 0.05f);
    }

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