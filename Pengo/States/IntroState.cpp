#include "IntroState.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "SceneManager.h"
#include "SnoBeeManager.h"

#include "Components/Blocks/EggBlockComponent.h"

#include "GameTime.h"


void dae::IntroState::OnEnter()
{
    auto& sceneManager = SceneManager::GetInstance();
    m_pGameScene = &sceneManager.CreateScene("Game");
    sceneManager.SetActiveScene("Game");

    m_pGrid = m_LevelLoader.LoadLevel(m_LevelIndex, *m_pGameScene, m_GameMode);

    m_pPlayer1 = m_LevelLoader.GetPlayerObject("Pengo");
    m_pPlayer2 = m_LevelLoader.GetPlayerObject("Pengo2");

    m_LevelLoader.LoadUI(*m_pGameScene, m_pPlayer1, m_pPlayer2);


    for (auto* eggObj : SnoBeeManager::GetInstance().GetEggs())
    {
        if (auto* egg = eggObj->GetComponent<EggBlockComponent>())
        {
            egg->SetFlashing(true);
        }
    }
}

void dae::IntroState::OnExit()
{
    for (auto* eggObj : SnoBeeManager::GetInstance().GetEggs())
    {
        if (auto* egg = eggObj->GetComponent<EggBlockComponent>())
        {
            egg->SetFlashing(false);
        }
    }
}

std::unique_ptr<dae::GameState> dae::IntroState::Update()
{
    for (auto* eggObj : SnoBeeManager::GetInstance().GetEggs())
    {
        if (auto* egg = eggObj->GetComponent<EggBlockComponent>())
        {
            egg->UpdateFlash();
        }
    }

    m_IntroTimer -= GameTime::GetInstance().GetDeltaTime();
    if (m_IntroTimer <= 0.f)
    {
        SnoBeeManager::GetInstance().HatchNextEgg();
        SnoBeeManager::GetInstance().HatchNextEgg();
        SnoBeeManager::GetInstance().HatchNextEgg();

        return std::make_unique<GameplayState>(
            m_pGameScene, m_pGrid, m_pPlayer1, m_pPlayer2, m_GameMode
        );
    }

    return nullptr;
}