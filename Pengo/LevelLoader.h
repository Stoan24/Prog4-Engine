#pragma once
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "Components/GridComponent.h"
#include <nlohmann/json_fwd.hpp>

using json = nlohmann::json;

namespace dae
{
    class LevelLoader
    {
    public:
        GridComponent* LoadLevel(const std::string& filePath, Scene& scene);

        GameObject* GetObject(const std::string& name) const
        {
            auto it = m_NamedObjects.find(name);
            if (it != m_NamedObjects.end()) return it->second;
            return nullptr;
        }

    private:
        std::unordered_map<std::string, GameObject*> m_NamedObjects;

        void LoadGrid(const json& gridJson, Scene& scene, GridComponent*& outGrid);
        void LoadCell(int id, int col, int row, Scene& scene, GridComponent* grid);
        void LoadUI(const json& uiJson, Scene& scene);
        void LoadSound();
    };
}