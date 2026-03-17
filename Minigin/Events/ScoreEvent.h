#pragma once
#include "GameComponent.h"
#include "EventListener.h"
#include "Components/TextComponent.h"
#include "SDBMHasher.h"
#include "EventManager.h"
#include "Steam/Achievement.h"

namespace dae
{
	class GameObject;
	class ScoreEvent final : public GameComponent, public EventListener
	{
	public:
		ScoreEvent(GameObject* gameObject, TextComponent* text, GameObject* trackedPlayer)
			:GameComponent(gameObject),
			m_pTextComponent{ text },
			m_pTrackedPlayer{ trackedPlayer }
		{
			EventManager::GetInstance().AddEvent(make_sdbm_hash("ScoreChange"), this);
			EventManager::GetInstance().AddEvent(make_sdbm_hash("Winner"), this);
		}

		void OnEvent(const Event& e) override
		{
			if (e.id == make_sdbm_hash("ScoreChange"))
			{
				if (e.args[0].gameObject == m_pTrackedPlayer)
				{
					std::string scoreStr = "Score: " + std::to_string(e.args[1].value);
					m_pTextComponent->SetText(scoreStr);
				}
			}
#ifdef USE_STEAMWORKS
			if (e.id == make_sdbm_hash("Winner"))
			{
				if (g_SteamAchievements)
					g_SteamAchievements->SetAchievement("ACH_WIN_100_GAMES");
			}
#endif // USE_STEAMWORKS

		}

	private:

		TextComponent* m_pTextComponent{ nullptr };

		//Change to Subject maybe? --> Whole of Scores can be put inside Observers, using Events for now to learn
		GameObject* m_pTrackedPlayer{ nullptr };
	};
}