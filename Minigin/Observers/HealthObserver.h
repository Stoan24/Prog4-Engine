#pragma once
#include "Observer.h"
#include "SDBMHasher.h"
#include "GameComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TextComponent.h"

namespace dae
{
	class HealthObserver : public GameComponent, public Observer
	{
		
	public:
		explicit HealthObserver(GameObject* gameObject, TextComponent* pText, Subject* pSubject)
			:GameComponent(gameObject),
			m_pTextComponent{ pText },
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

		void OnSubjectDestroyed() override { m_pSubject = nullptr; }

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