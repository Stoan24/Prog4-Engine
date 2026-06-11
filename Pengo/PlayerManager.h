#pragma once
#include "Singleton.h"
#include "Events/Event.h"
#include "Observers/Observer.h"
#include <vector>
#include "SDBMHasher.h"

namespace dae
{
    struct PlayerData
    {
        int lives{ 4 };
        int score{ 0 };
        int playerIdx{ 0 };
    };

    class PlayerManager final : public Singleton<PlayerManager>, public Observer
    {
    public:
        void Initialize(int playerCount);

        void Notify(const Event& e) override;

        void RegisterPlayer(int playerIdx, GameObject* playerObject);
        int GetPlayerIndex(GameObject* playerObject) const;

        void SetLives(int playerIdx, int lives) { m_Players[playerIdx].lives = lives; }
        int GetLives(int playerIdx) const { return m_Players[playerIdx].lives; }

        void SetScore(int playerIdx, int score) { m_Players[playerIdx].score = score; }
        int GetScore(int playerIdx) const { return m_Players[playerIdx].score; }
        void AddScore(int playerIdx, int score);

        int GetPlayerCount() const { return static_cast<int>(m_Players.size()); }

    private:
        friend class Singleton<PlayerManager>;
        PlayerManager() = default;

        std::vector<PlayerData> m_Players;
        std::vector<GameObject*> m_PlayerObjects;
    };
}