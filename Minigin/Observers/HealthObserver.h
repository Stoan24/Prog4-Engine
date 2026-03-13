#pragma once
#include "Observer.h"
#include "SDBMHasher.h"
#include "Components/HealthComponent.h"
#include "Components/TextComponent.h"

namespace dae
{
	class HealthObserver : public Observer
	{
		
	public:
		explicit HealthObserver(TextComponent* pText, Subject* pSubject)
			:m_pTextComponent{ pText },
			m_pSubject{ pSubject }
		{
		}

		virtual ~HealthObserver()
		{
			if (m_pSubject) m_pSubject->removeObserver(this);
		}

		void Notify(GameObject* gameObject, EventId event) override
		{
			if (event == make_sdbm_hash("PlayerHit"))
			{
				auto health = gameObject->GetComponent<HealthComponent>();
				int currentLives = health->GetLives();
				UpdateDisplay(currentLives);
			}
		}

	private:
		TextComponent* m_pTextComponent{ nullptr };
		Subject* m_pSubject{ nullptr };

		void UpdateDisplay(int lives)
		{
			std::string healthStr = "# Lives: " + std::to_string(lives);
			m_pTextComponent->SetText(healthStr);
		}

	};
}