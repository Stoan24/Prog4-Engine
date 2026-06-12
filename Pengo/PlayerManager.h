#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include <vector>

namespace dae
{
    struct PlayerData
    {
        int lives{ 4 };
        int playerIdx{ 0 };
    };

    class PlayerManager final : public Singleton<PlayerManager>
    {
    public:
        void Initialize(int playerCount);

        void RegisterPlayer(int playerIdx, GameObject* playerObject);
        int GetPlayerIndex(GameObject* playerObject) const;
        GameObject* GetPlayerObject(int playerIdx) const;

        void SetLives(int playerIdx, int lives) { m_Players[playerIdx].lives = lives; }
        int GetLives(int playerIdx) const { return m_Players[playerIdx].lives; }

        int GetPlayerCount() const { return static_cast<int>(m_Players.size()); }

    private:
        friend class Singleton<PlayerManager>;
        PlayerManager() = default;

        std::vector<PlayerData> m_Players;
        std::vector<GameObject*> m_PlayerObjects;
    };
}