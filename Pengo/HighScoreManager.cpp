#include "HighScoreManager.h"

#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

void dae::HighScoreManager::Load()
{
    m_Entries.clear();

    std::ifstream file(m_filePath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::istringstream ss(line);
        HighScoreEntry e;
        ss >> e.name >> e.score;

        if (ss.fail()) continue;

        if (e.name.size() > 3) e.name.resize(3);
        while (e.name.size() < 3) e.name += 'A';

        m_Entries.push_back(e);
    }

    //Best first
    std::sort(m_Entries.begin(), m_Entries.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b) { return a.score > b.score; });
}

void dae::HighScoreManager::Save() const
{
    std::ofstream file(m_filePath);
    if (!file.is_open()) return;


    for (const auto& e : m_Entries)
    {
        file << e.name << " " << e.score << "\n";
    }
}

bool dae::HighScoreManager::IsHighScore(int score) const
{
    if (static_cast<int>(m_Entries.size()) < MaxEntries) return true;

    return score > m_Entries.back().score;
}

void dae::HighScoreManager::AddEntry(const std::string& name, int score)
{
    m_Entries.push_back({ name, score });

    std::sort(m_Entries.begin(), m_Entries.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b) { return a.score > b.score; });

    if (static_cast<int>(m_Entries.size()) > MaxEntries)
    {
        m_Entries.resize(MaxEntries);
    }

    Save();
}