#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* gameObject)
	: GameComponent(gameObject)
{
}

void dae::ScoreComponent::AddPoints(int amount)
{
	m_CurrentScore += amount;
}
