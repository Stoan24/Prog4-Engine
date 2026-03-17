#pragma once

#if USE_STEAMWORKS
#include <steamtypes.h>
#include <steam_api.h>


namespace dae
{
#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
	struct Achievement
	{
		int m_eAchievementID;
		const char* m_pchAchievementID;
		char m_rgchName[128];
		char m_rgchDescription[256];
		bool m_bAchieved;
		int m_iIconImage;
	};

	// Defining our achievements
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	// Achievement array which will hold data about the achievements and their state
	inline Achievement g_Achievements[] =
	{
		_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
	};



	class SteamAchievements
	{
	private:
		int64 m_iAppID; // Our current AppID
		Achievement* m_pAchievements; // Achievements data
		int m_iNumAchievements; // The number of Achievements
		bool m_bInitialized; // Are we ready to use the API?

	public:
		SteamAchievements(Achievement* Achievements, int NumAchievements);
		~SteamAchievements() = default;

		bool Initialize();
		bool SetAchievement(const char* ID);

		//STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		//STEAM_CALLBACK(SteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
	};
	
	// Global access to Achievements object
	inline SteamAchievements* g_SteamAchievements = NULL;
}
#endif
