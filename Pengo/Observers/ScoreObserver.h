#pragma once
#include "Observers/Observer.h"
#include "GameComponent.h"
#include "SDBMHasher.h"
#include "../Components/ScoreComponent.h"
#include "Components/TextComponent.h"

namespace dae
{
    class ScoreObserver final : public GameComponent, public Observer
    {
    public:
        ScoreObserver(GameObject* gameObject, TextComponent* pText, ScoreComponent* pScore)
            : GameComponent(gameObject),
            m_pTextComponent{ pText },
            m_pScore{ pScore },
            m_pSubject{ pScore->GetSubject() }
        {
            m_pSubject->AddObserver(this);
        }

        ~ScoreObserver()
        {
            if (m_pSubject) m_pSubject->RemoveObserver(this);
        }

        void Notify(const Event& e) override
        {
            if (e.id == make_sdbm_hash("ScoreChanged"))
            {
                m_pTextComponent->SetText("Score: " + std::to_string(m_pScore->GetScore()));
            }
        }

    private:
        TextComponent* m_pTextComponent{ nullptr };
        ScoreComponent* m_pScore{ nullptr };
        Subject* m_pSubject{ nullptr };
    };
}