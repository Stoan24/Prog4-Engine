#pragma once
#include "Singleton.h"

namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
		float GetDeltaTime() const { return m_deltaTime; }

		void SetFixedDeltaTime(float fixedDeltaTime) { m_fixedDeltaTime = fixedDeltaTime; }
		float GetFixedDeltaTime() const { return m_fixedDeltaTime; }

	private:
		float m_deltaTime{ 0.f };
		float m_fixedDeltaTime{ 0.f };
	};
}
