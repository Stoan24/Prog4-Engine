#pragma once
#include "Observers/Observer.h"
#include "SDBMHasher.h"
#include "GameComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TextComponent.h"

namespace dae
{
	class HealthObserver : public GameComponent, public Observer
	{
		
	public:
		explicit HealthObserver(GameObject* gameObject, TextComponent* pText, HealthComponent* pHealth)
			:GameComponent(gameObject),
			m_pTextComponent{ pText },
			m_pHealth{ pHealth },
			m_pSubject{ pHealth->GetSubject() }
		{
			if (m_pSubject) m_pSubject->AddObserver(this);
		}

		virtual ~HealthObserver()
		{
			if (m_pSubject) m_pSubject->RemoveObserver(this);
		}

		void Notify(const Event& e) override
		{
			if (e.id == make_sdbm_hash("PlayerHit"))
			{
				m_pTextComponent->SetText("# Lives: " + std::to_string(m_pHealth->GetLives()));

			}
		}

	private:
		TextComponent* m_pTextComponent{ nullptr };
		HealthComponent* m_pHealth{ nullptr };
		Subject* m_pSubject{ nullptr };

		void UpdateDisplay(int lives)
		{
			std::string healthStr = "# Lives: " + std::to_string(lives);
			m_pTextComponent->SetText(healthStr);
		}

	};
}