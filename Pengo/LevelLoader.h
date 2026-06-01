#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>

#include "Scene.h"
#include "Components/GridComponent.h"
#include "States/StartMenuState.h"


using json = nlohmann::json;

namespace dae
{
    class LevelLoader
    {
    public:
        GridComponent* LoadLevel(int levelIndex, Scene& scene, GameMode gameMode);


        static int GetLevelCount(const std::string& filePath);
        static void LoadSounds(const std::string& filePath);
        void LoadUI(Scene& scene, GameObject* player1, GameObject* player2);


        GameObject* GetObject(const std::string& name) const
        {
            auto it = m_NamedObjects.find(name);
            if (it != m_NamedObjects.end()) return it->second;
            return nullptr;
        }

        static std::string GetLevelsPath() { return "Data/JSON/levels.json"; }

    private:
        GameMode m_GameMode{ GameMode::SinglePlayer };
        std::unordered_map<std::string, GameObject*> m_NamedObjects;

        void LoadGrid(const json& gridJson, Scene& scene, GridComponent*& outGrid);
        void LoadCell(int id, int col, int row, Scene& scene, GridComponent* grid); 
    };
}