#pragma once
#include "ScoreManager.h"
#include "Components/ScoreComponent.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"
#include <vector>

void dae::ScoreManager::Initialize()
{
	EventManager::GetInstance().AddEvent(make_sdbm_hash("AddScore"), this);
}


void dae::ScoreManager::AddScoreToTrack(ScoreComponent* score)
{
	m_pScores.push_back(score);
}

void dae::ScoreManager::RemoveScoreToTrack(ScoreComponent* score)
{
	std::erase(m_pScores, score);
}

void dae::ScoreManager::OnEvent(const Event& e)
{
	if (e.id == make_sdbm_hash("AddScore"))
	{
		for (auto* score : m_pScores)
		{
			if (score->GetGameObject() == e.args[0].gameObject)
			{
				score->AddPoints(e.args[1].value);
				break;
			}
		}
	}
}
