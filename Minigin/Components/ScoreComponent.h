#pragma once
#include "GameComponent.h"

namespace dae
{

	class ScoreComponent final : public GameComponent
	{
	public:
		ScoreComponent(GameObject* gameObject);

		virtual ~ScoreComponent();

		int GetScore() const { return m_CurrentScore; }

		void AddPoints(int amount);
		void ResetScore() { m_CurrentScore = 0; }
		
		GameObject* GetGameObject() const override { return GameComponent::GetGameObject(); }

	private:
		int m_CurrentScore{ 0 };
		int m_WinnerScore{ 500 };
	};

}