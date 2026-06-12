#pragma once
#include "GameComponent.h"

namespace dae
{

	class ScoreComponent final : public GameComponent
	{
	public:
		ScoreComponent(GameObject* gameObject);
		virtual ~ScoreComponent() = default;

		void AddPoints(int amount);
		void ResetScore() { m_CurrentScore = 0; }

		int GetScore() const { return m_CurrentScore; }
		void SetScore(int score) { m_CurrentScore = score; }

		GameObject* GetGameObject() const override { return GameComponent::GetGameObject(); }

	private:

		int m_CurrentScore{ 0 };
	};

}