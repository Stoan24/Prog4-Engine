#pragma once
#include "Singleton.h"
#include "Observers/Observer.h"

#include <vector>

namespace dae
{
    class ScoreManager final : public Singleton<ScoreManager>, public IObserver
    {
    public:
        void Initialize();

        void Notify(const Event& e) override;

        int GetScore(int playerIdx) const;
        void SetScore(int playerIdx, int score);
        void AddScore(int playerIdx, int amount);
        void ResetScore() { m_scores = { 0,0 }; };

    private:
        friend class Singleton<ScoreManager>;
        ScoreManager() = default;

        int CalculatePoints(const Event& e) const;
        int CalculateTimeBonus(int seconds) const;

        std::vector<int> m_scores{ 0, 0 };

        //Scores
        const std::vector<int> m_KillScores{ 0, 400, 1600, 3200, 6400 };
        const int m_MaxKills{ 4 };

        const int m_StunSideScore{ 5000 };
        const int m_StunMiddleScore{ 10000 };

        const int m_EggBreakScore{ 500 };

        const int m_IceBlockScore{ 30 };
    };
}