#pragma once
#include "Singleton.h"
#include <vector>
#include "Events/Event.h"
#include "Observers/Observer.h"

namespace dae
{
	class ScoreComponent;

	class ScoreManager final : public Singleton<ScoreManager>, public Observer
	{
	public:
		void Initialize();

		void Notify(const Event& e) override;

	private:
		friend class Singleton<ScoreManager>;
		ScoreManager() = default;

		std::vector<ScoreComponent*> m_pScores;

		int m_killScore{ 100 };
		int m_stunScore{ 50 };
	};
}