#pragma once
#include "GameComponent.h"
#include "Observers/Observer.h"
#include "Observers/Subject.h"

#include <memory>
#include <glm/vec2.hpp>


namespace dae
{
	class HealthComponent final : public GameComponent, public IObserver
	{
	public:
		explicit HealthComponent(GameObject* gameObject, int lives);
		virtual ~HealthComponent();

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		virtual void Update() override;

		void Notify(const Event& e) override;

		void TakeDamage(int amount);
		void RestoreLives();
		bool IsDead();

		int GetLives() const { return m_Lives; }
		void SetLives(int lives) { m_Lives = lives; }

		void SetSpawnCell(glm::ivec2 cell) { m_SpawnCell = cell; }

		Subject* GetSubject() const { return m_pSubject.get(); }

	private:

		void Respawn();

		int m_TotalLives{};
		int m_Lives{};
		bool m_IsDead{ false };
		
		bool m_IsInvincible{ false };
		float m_InvincibleTimer = 0.0f;
		const float m_MaxInvincibleTime = 2.0f;

		glm::ivec2 m_SpawnCell{ 1, 1 };

		std::unique_ptr<Subject> m_pSubject;
	};
}