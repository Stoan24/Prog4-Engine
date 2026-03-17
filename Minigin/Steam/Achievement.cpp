
#if USE_STEAMWORKS
#include "Achievement.h"

dae::SteamAchievements::SteamAchievements(Achievement* Achievements, int NumAchievements)
	:m_iAppID(0),
	m_bInitialized(false)
	//m_CallbackUserStatsStored(this, &CSteamAchievements::OnUserStatsStored),
	//m_CallbackAchievementStored(this, &CSteamAchievements::OnAchievementStored)
{
	m_iAppID = SteamUtils()->GetAppID();
	m_pAchievements = Achievements;
	m_iNumAchievements = NumAchievements;
	m_bInitialized = Initialize();
}

bool dae::SteamAchievements::Initialize()
{
	// Is Steam loaded? If not we can't get stats.
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// Is the user logged on?  If not we can't access stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}

	return true;
}

bool dae::SteamAchievements::SetAchievement(const char* ID)
{
	// Have we received a call back from Steam yet?
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

#endif