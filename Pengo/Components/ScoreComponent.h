#pragma once
#include "GameComponent.h"
#include <Observers/Subject.h>

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

		
		GameObject* GetGameObject() const override { return GameComponent::GetGameObject(); }
		Subject* GetSubject() const { return m_pSubject.get(); }

	private:
		std::unique_ptr<Subject> m_pSubject;

		int m_CurrentScore{ 0 };
		//int m_WinnerScore{ 500 };
	};

}