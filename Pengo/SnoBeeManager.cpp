#include "SnoBeeManager.h"
#include "Components/Blocks/EggBlockComponent.h"
#include "Components/SnoBeeComponent.h"
#include "SDBMHasher.h"
#include <algorithm>
#include <Components/TextureComponent.h>
#include "SceneManager.h"
#include "Scene.h"

void dae::SnoBeeManager::Initialize(GridComponent* grid)
{
    m_pGrid = grid;
    m_pScene = SceneManager::GetInstance().GetActiveScene();

    EventManager::GetInstance().AddEvent(make_sdbm_hash("EnemyKilled"), this);
}

void dae::SnoBeeManager::RegisterSnoBee(GameObject* snoBee)
{
    m_ActiveSnoBees.push_back(snoBee);
}

void dae::SnoBeeManager::RegisterEgg(GameObject* eggBlock)
{
    m_Eggs.push_back(eggBlock);
}

void dae::SnoBeeManager::CreateSnoBee(int col, int row)
{
    auto snoBee = std::make_unique<dae::GameObject>();
    snoBee->AddComponent<dae::TextureComponent>()->SetTexture("Snobee.png");
    snoBee->AddComponent<dae::GridMoveComponent>(m_pGrid, col, row, 1.f);
    snoBee->AddComponent<dae::SnoBeeComponent>(m_pGrid);
    snoBee->AddComponent<dae::CollisionComponent>()->SetSize(16, 16);
    snoBee->AddTag("Enemy");
    snoBee->SetParent(m_pGrid->GetGameObject(), false);

    RegisterSnoBee(snoBee.get());

    m_pScene->Add(std::move(snoBee));
}

void dae::SnoBeeManager::Notify(const Event& e)
{
    if (e.id == make_sdbm_hash("EnemyKilled"))
    {
        CleanupDead();

        HatchNextEgg();
    }
}

void dae::SnoBeeManager::CleanupDead()
{
    m_ActiveSnoBees.erase(
        std::remove_if(m_ActiveSnoBees.begin(), m_ActiveSnoBees.end(),
            [](GameObject* obj) { return obj->IsMarkedForDestruction(); }),
        m_ActiveSnoBees.end()
    );
}

void dae::SnoBeeManager::HatchNextEgg()
{
    for (auto it = m_Eggs.begin(); it != m_Eggs.end(); ++it)
    {
        GameObject* eggObj = *it;
        if (eggObj->IsMarkedForDestruction()) continue;

        auto* egg = eggObj->GetComponent<EggBlockComponent>();
        if (!egg || egg->HasHatched()) continue;

        
        auto* move = eggObj->GetComponent<GridMoveComponent>();
        if (!move) continue;

        glm::ivec2 cell = move->GetCurrentCell();

        
        egg->Hatch();

        
        CreateSnoBee(cell.x, cell.y);


        m_Eggs.erase(it);
        return;
    }
}