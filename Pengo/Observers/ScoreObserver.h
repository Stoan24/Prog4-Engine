#pragma once
#include "GameComponent.h"
#include "Observers/Observer.h"

#include "../Components/ScoreComponent.h"
#include "Components/TextComponent.h"

#include "Events/EventManager.h"

#include "SDBMHasher.h"

namespace dae
{
    //Should be named ScoreDisplay
    class ScoreObserver final : public GameComponent, public IObserver
    {
    public:
        explicit ScoreObserver(GameObject* gameObject, TextComponent* pText, ScoreComponent* pScore)
            : GameComponent(gameObject),
            m_pTextComponent{ pText },
            m_pScore{ pScore }
        {
            EventManager::GetInstance().AddEvent(make_sdbm_hash("EnemyKilled"), this);
            EventManager::GetInstance().AddEvent(make_sdbm_hash("StunEnemies"), this);
            EventManager::GetInstance().AddEvent(make_sdbm_hash("EggDestroyed"), this);
            EventManager::GetInstance().AddEvent(make_sdbm_hash("IceBlockDestroyed"), this);
            EventManager::GetInstance().AddEvent(make_sdbm_hash("LevelFinish"), this);
        }

        virtual ~ScoreObserver()
        {
            EventManager::GetInstance().RemoveObserver(make_sdbm_hash("EnemyKilled"), this);
            EventManager::GetInstance().RemoveObserver(make_sdbm_hash("StunEnemies"), this);
            EventManager::GetInstance().RemoveObserver(make_sdbm_hash("EggDestroyed"), this);
            EventManager::GetInstance().RemoveObserver(make_sdbm_hash("IceBlockDestroyed"), this);
            EventManager::GetInstance().RemoveObserver(make_sdbm_hash("LevelFinish"), this);
        }

        void Notify(const Event& e) override
        {
            if (e.id == make_sdbm_hash("EnemyKilled") || e.id == make_sdbm_hash("StunEnemies")
                || e.id == make_sdbm_hash("EggDestroyed") || e.id == make_sdbm_hash("IceBlockDestroyed")
                || e.id == make_sdbm_hash("LevelFinish"))
            {
                m_pTextComponent->SetText("Score: " + std::to_string(m_pScore->GetScore()));
            }
        }

    private:
        TextComponent* m_pTextComponent{ nullptr };
        ScoreComponent* m_pScore{ nullptr };
    };
}