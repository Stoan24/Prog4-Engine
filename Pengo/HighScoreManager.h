#pragma once
#include <string>
#include <vector>
#include "Singleton.h"

namespace dae
{
    struct HighScoreEntry
    {
        std::string name{ "AAA" };  //Always 3 characters
        int score{ 0 };
    };

    class HighScoreManager final : public Singleton<HighScoreManager>
    {
    public:
        static constexpr int MaxEntries = 5;

        void Load();
        void Save() const;

        bool IsHighScore(int score) const;

        void AddEntry(const std::string& name, int score);

        const std::vector<HighScoreEntry>& GetEntries() const { return m_Entries; }

    private:
        friend class Singleton<HighScoreManager>;
        HighScoreManager() = default;

        std::vector<HighScoreEntry> m_Entries;

        const std::string m_filePath = "HighScores.txt";
    };
}