#include "LevelLoader.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

//Engine components
#include "GameObject.h"
#include "Components/TextureComponent.h"
#include "Components/TextComponent.h"
#include "Components/GridComponent.h"
#include "Components/GridMoveComponent.h"
#include "Components/CollisionComponent.h"
#include "ResourceManager.h"

#include "Transform.h"

//Game components
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/Blocks/IceBlockComponent.h"
#include "Components/Blocks/DiamondBlockComponent.h"
#include "Components/Blocks/EggBlockComponent.h"
#include "Components/SnoBeeComponent.h"
#include "Observers/HealthObserver.h"
#include "Observers/ScoreObserver.h"

#include "SnoBeeManager.h"

using json = nlohmann::json;

dae::GridComponent* dae::LevelLoader::LoadLevel(const std::string& filePath, Scene& scene)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("LevelLoader: could not open " + filePath);
    }

    json data = json::parse(file);

    GridComponent* grid = nullptr;

    LoadGrid(data["grid"], scene, grid);

    LoadUI(data["ui"], scene);

    SnoBeeManager::GetInstance().Initialize(grid);
    SnoBeeManager::GetInstance().HatchNextEgg();
    SnoBeeManager::GetInstance().HatchNextEgg();
    SnoBeeManager::GetInstance().HatchNextEgg();

    return grid;
}

void dae::LevelLoader::LoadGrid(const json& gridJson, Scene& scene, GridComponent*& outGrid)
{
    int cellSize = gridJson["cellSize"];

    glm::vec2 origin{ 0.f, 0.f };
    if (gridJson.contains("origin"))
    {
        origin.x = gridJson["origin"]["x"];
        origin.y = gridJson["origin"]["y"];
    }

    const auto& layout = gridJson["layout"];
    int rows = static_cast<int>(layout.size());
    int cols = (rows > 0) ? static_cast<int>(layout[0].size()) : 0;

    auto gridObject = std::make_unique<GameObject>();
    gridObject->GetComponent<Transform>()->SetLocalPosition(origin.x, origin.y);
    outGrid = gridObject->AddComponent<GridComponent>(cols, rows, cellSize, glm::vec2{ 0.f, 0.f });

    if (gridJson.contains("texture"))
    {
        gridObject->AddComponent<TextureComponent>()->SetTexture(gridJson["texture"]);
    }

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int tileID = layout[r][c];
            if (tileID > 0)
            {
                LoadCell(tileID, c, r, scene, outGrid);
            }
        }
    }

    scene.Add(std::move(gridObject));
}

void dae::LevelLoader::LoadCell(int id, int col, int row, Scene& scene, GridComponent* grid)
{
    auto gameObject = std::make_unique<GameObject>();

    switch (id)
    {
    case 1: //Ice Block
        gameObject->AddComponent<TextureComponent>()->SetTexture("IceBlock.png");
        gameObject->AddComponent<GridMoveComponent>(grid, col, row, 10.f);
        gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);

        gameObject->AddComponent<IceBlockComponent>(grid);
        break;

    case 2: //Diamond Block
        gameObject->AddComponent<TextureComponent>()->SetTexture("DiamondBlock.png");
        gameObject->AddComponent<GridMoveComponent>(grid, col, row, 10.f);
        gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);

        gameObject->AddComponent<DiamondBlockComponent>(grid);
        break;

    case 3: //Player Spawn
        gameObject->AddComponent<TextureComponent>()->SetTexture("Pengo.png");
        gameObject->AddComponent<GridMoveComponent>(grid, col, row, 1.5f);
        gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);

        gameObject->AddComponent<HealthComponent>(3);
        gameObject->AddComponent<ScoreComponent>();
        gameObject->AddTag("Player");

        m_NamedObjects["Pengo"] = gameObject.get();
        break;

    case 4: //Ice Block Enemy
        gameObject->AddComponent<TextureComponent>()->SetTexture("IceBlock.png");
        gameObject->AddComponent<GridMoveComponent>(grid, col, row, 10.f);
        gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);

        gameObject->AddComponent<EggBlockComponent>(grid);

        SnoBeeManager::GetInstance().RegisterEgg(gameObject.get());

        gameObject->AddTag("Enemy");
        break;

    case 5: //Player 2
        gameObject->AddComponent<TextureComponent>()->SetTexture("Pengo2.png");
        gameObject->AddComponent<GridMoveComponent>(grid, col, row, 1.5f);
        gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);

        gameObject->AddComponent<HealthComponent>(3);
        gameObject->AddComponent<ScoreComponent>();
        //gameObject->AddTag("Player");

        m_NamedObjects["Pengo2"] = gameObject.get();
        break;
    }

    if (gameObject->HasComponent<TextureComponent>())
    {
        gameObject->SetParent(grid->GetGameObject(), false);
        scene.Add(std::move(gameObject));
    }
}

void dae::LevelLoader::LoadUI(const json& uiJson, Scene& scene)
{
    for (const auto& item : uiJson)
    {
        std::string type = item["type"];
        std::string fontFile = item["font"]["file"];
        uint8_t fontSize = item["font"]["size"];
        std::string initialText = item["initialText"];

        float x = item["position"]["x"];
        float y = item["position"]["y"];

        auto font = ResourceManager::GetInstance().LoadFont(fontFile, fontSize);

        auto gameObject = std::make_unique<GameObject>();
        gameObject->GetComponent<Transform>()->SetLocalPosition(x, y);
        gameObject->AddComponent<TextureComponent>();
        auto text = gameObject->AddComponent<TextComponent>(initialText, font);


        std::string tracksPlayer = item["tracksPlayer"];
        GameObject* player = m_NamedObjects.at(tracksPlayer);

        if (type == "healthObserver")
        {
            auto* health = player->GetComponent<HealthComponent>();
            gameObject->AddComponent<HealthObserver>(text, health);
        }
        else if (type == "scoreObserver")
        {
            auto* score = player->GetComponent<ScoreComponent>();
            gameObject->AddComponent<ScoreObserver>(text, score);
        }

        scene.Add(std::move(gameObject));
    }
}