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


        GameObject* GetPlayerObject(const std::string& name) const
        {
            auto it = m_PlayerObjects.find(name);
            if (it != m_PlayerObjects.end()) return it->second;
            return nullptr;
        }

        static std::string GetLevelsPath() { return "Data/JSON/levels.json"; }

    private:
        GameMode m_GameMode{ GameMode::SinglePlayer };
        std::unordered_map<std::string, GameObject*> m_PlayerObjects;

        void LoadGrid(const json& gridJson, Scene& scene, GridComponent*& outGrid);
        void LoadCell(int id, int col, int row, Scene& scene, GridComponent* grid); 

        std::unique_ptr<GameObject> CreateIceBlock(GridComponent* grid, int col, int row);
        std::unique_ptr<GameObject> CreateDiamondBlock(GridComponent* grid, int col, int row);
        std::unique_ptr<GameObject> CreatePlayer1(GridComponent* grid, int col, int row);
        std::unique_ptr<GameObject> CreatePlayer2(GridComponent* grid, int col, int row);
        std::unique_ptr<GameObject> CreateEggBlock(GridComponent* grid, int col, int row);
    };
}