#pragma once
#include "GameComponent.h"
#include <memory>
#include <Subject.h>

namespace dae
{
	class HealthComponent final : public GameComponent
	{
	public:
		explicit HealthComponent(GameObject* gameObject, int lives);
		virtual ~HealthComponent() = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void TakeDamage(int amount);
		void RestoreLives();
		bool IsDead();

		int GetLives() const { return m_Lives; }

		Subject* GetSubject() const { return m_pSubject.get(); }

	private:
		int m_TotalLives{};
		int m_Lives{};
		bool m_IsDead{ false };

		std::unique_ptr<Subject> m_pSubject;
	};
}