#pragma once
#include "Singleton.h"
#include <vector>
#include "Events/EventListener.h"
#include "Events/Event.h"


namespace dae
{
	class ScoreComponent;

	class ScoreManager final : public Singleton<ScoreManager>, public EventListener
	{
	public:
		void Initialize();

		void AddScoreToTrack(ScoreComponent* score);
		void RemoveScoreToTrack(ScoreComponent* score);

		void OnEvent(const Event& e) override;

	private:
		friend class Singleton<ScoreManager>;
		ScoreManager() = default;

		std::vector<ScoreComponent*> m_pScores;
	};
}