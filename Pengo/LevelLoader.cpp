#include "LevelLoader.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

//Engine
#include "GameObject.h"
#include "Components/TextureComponent.h"
#include "Components/TextComponent.h"
#include "Components/GridComponent.h"
#include "Components/GridMoveComponent.h"
#include "Components/CollisionComponent.h"
#include "Transform.h"
#include "ServiceLocator.h"

//Managers
#include "ResourceManager.h"
#include "PlayerManager.h"
#include "ScoreManager.h"
#include "SnoBeeManager.h"


//Game
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/Blocks/IceBlockComponent.h"
#include "Components/Blocks/DiamondBlockComponent.h"
#include "Components/Blocks/EggBlockComponent.h"
#include "Observers/HealthObserver.h"
#include "Observers/ScoreObserver.h"


using json = nlohmann::json;

dae::GridComponent* dae::LevelLoader::LoadLevel(int levelIndex, Scene& scene, GameMode gameMode)
{
    m_GameMode = gameMode;

    SnoBeeManager::GetInstance().Clear();
    m_PlayerObjects.clear();

    std::ifstream file(GetLevelsPath());
    if (!file.is_open())
    {
        throw std::runtime_error("LevelLoader: could not open levels.json" );
    }

    json data = json::parse(file);
    const auto& levelJson = data["levels"][levelIndex];

    GridComponent* grid = nullptr;
    LoadGrid(levelJson, scene, grid);

    SnoBeeManager::GetInstance().Initialize(grid);

    return grid;
}

int dae::LevelLoader::GetLevelCount(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open()) return 0;
    json data = json::parse(file);
    
    return static_cast<int>(data["levels"].size());
}

void dae::LevelLoader::LoadSounds(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("LevelLoader: could not open " + filePath);
    }

    json data = json::parse(file);
    auto& ss = ServiceLocator::GetSoundSystem();

    for (const auto& entry : data["sounds"])
    {
        std::string name = entry["name"];
        std::string path = entry["file"];
        ss.RegisterSound(make_sdbm_hash(name.c_str()), path);
    }

    ss.SetVolume(static_cast<float>(data["volume"]));
}

void dae::LevelLoader::LoadUI(Scene& scene, GameObject* player1, GameObject* player2)
{
    auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);

    auto makeObservers = [&](GameObject* player, float x)
    {
        if (!player) return;

        auto* health = player->GetComponent<HealthComponent>();
        auto* score = player->GetComponent<ScoreComponent>();

        //Health display
        auto healthObj = std::make_unique<GameObject>();
        healthObj->GetComponent<Transform>()->SetLocalPosition(x, 0.f);
        healthObj->AddComponent<TextureComponent>();

        auto healthText = healthObj->AddComponent<TextComponent>("", font);
        healthObj->AddComponent<HealthObserver>(healthText, health);
        healthText->SetText("# Lives: " + std::to_string(health->GetLives()));

        scene.Add(std::move(healthObj));

        //Score display
        if (score)
        {
            auto scoreObj = std::make_unique<GameObject>();
            scoreObj->GetComponent<Transform>()->SetLocalPosition(x, 16.f);
            scoreObj->AddComponent<TextureComponent>();

            auto scoreText = scoreObj->AddComponent<TextComponent>("", font);
            scoreObj->AddComponent<ScoreObserver>(scoreText, score);
            scoreText->SetText("Score: " + std::to_string(score->GetScore()));

            scene.Add(std::move(scoreObj));
        }
    };

    makeObservers(player1, 0.f);
    makeObservers(player2, 100.f);
}

void dae::LevelLoader::LoadGrid(const json& gridJson, Scene& scene, GridComponent*& outGrid)
{
    auto backgroundObject = std::make_unique<GameObject>();
    backgroundObject->GetComponent<Transform>()->SetLocalPosition(0.f, 0.f);
    backgroundObject->AddComponent<TextureComponent>()->SetTexture("mainBackground.png");
    scene.Add(std::move(backgroundObject));


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
    case 1:
        gameObject = CreateIceBlock(grid, col, row);
        break;

    case 2: 
        gameObject = CreateDiamondBlock(grid, col, row); 
        break;

    case 3: 
        gameObject = CreatePlayer1(grid, col, row);
        break;

    case 4: 
        gameObject = CreateEggBlock(grid, col, row); 
        break;

    case 5: 
        gameObject = CreatePlayer2(grid, col, row); 
        break;

    default: 
        break;
    }

    if (gameObject && gameObject->HasComponent<TextureComponent>())
    {
        gameObject->SetParent(grid->GetGameObject(), false);
        scene.Add(std::move(gameObject));
    }
}

std::unique_ptr<dae::GameObject> dae::LevelLoader::CreateIceBlock(GridComponent* grid, int col, int row)
{
    auto gameObject = std::make_unique<GameObject>();
    gameObject->AddComponent<TextureComponent>()->SetTexture("IceBlock.png");
    gameObject->AddComponent<GridMoveComponent>(grid, col, row, 10.f);
    gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);
    gameObject->AddComponent<IceBlockComponent>(grid);
    return gameObject;
}

std::unique_ptr<dae::GameObject> dae::LevelLoader::CreateDiamondBlock(GridComponent* grid, int col, int row)
{
    auto gameObject = std::make_unique<GameObject>();
    gameObject->AddComponent<TextureComponent>()->SetTexture("DiamondBlock.png");
    gameObject->AddComponent<GridMoveComponent>(grid, col, row, 10.f);
    gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);
    gameObject->AddComponent<DiamondBlockComponent>(grid);
    return gameObject;
}

std::unique_ptr<dae::GameObject> dae::LevelLoader::CreatePlayer1(GridComponent* grid, int col, int row)
{
    auto gameObject = std::make_unique<GameObject>();
    gameObject->AddComponent<TextureComponent>()->SetTexture("Pengo.png");
    gameObject->AddComponent<GridMoveComponent>(grid, col, row, 2.f);
    gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);

    
    auto* health = gameObject->AddComponent<HealthComponent>(4);
    health->SetSpawnCell({col, row});
    int savedLives = PlayerManager::GetInstance().GetLives(0);
    if (savedLives > 0) health->SetLives(savedLives);

    auto* score = gameObject->AddComponent<ScoreComponent>();
    int savedScore = ScoreManager::GetInstance().GetScore(0);
    if (savedScore > 0) score->SetScore(savedScore);

    gameObject->AddTag("Player");


    m_PlayerObjects["Pengo"] = gameObject.get();
    PlayerManager::GetInstance().RegisterPlayer(0, gameObject.get());

    return gameObject;
}

std::unique_ptr<dae::GameObject> dae::LevelLoader::CreateEggBlock(GridComponent* grid, int col, int row)
{
    auto gameObject = std::make_unique<GameObject>();
    gameObject->AddComponent<TextureComponent>()->SetTexture("IceBlock.png");
    gameObject->AddComponent<GridMoveComponent>(grid, col, row, 10.f);
    gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);
    gameObject->AddComponent<EggBlockComponent>(grid);
    gameObject->AddTag("Enemy");

    SnoBeeManager::GetInstance().RegisterEgg(gameObject.get());
    return gameObject;
}

std::unique_ptr<dae::GameObject> dae::LevelLoader::CreatePlayer2(GridComponent* grid, int col, int row)
{
    if (m_GameMode == GameMode::SinglePlayer) return nullptr;

    auto gameObject = std::make_unique<GameObject>();
    gameObject->AddComponent<GridMoveComponent>(grid, col, row, 2.f);
    gameObject->AddComponent<CollisionComponent>()->SetSize(16, 16);


    if (m_GameMode == GameMode::Coop)
    {
        gameObject->AddComponent<TextureComponent>()->SetTexture("Pengo2.png");
        auto* score = gameObject->AddComponent<ScoreComponent>();
        int savedScore = ScoreManager::GetInstance().GetScore(1);
        if (savedScore > 0) score->SetScore(savedScore);

        gameObject->AddTag("Player");
    }
    else if (m_GameMode == GameMode::Versus)
    {
        gameObject->AddComponent<TextureComponent>()->SetTexture("SnoBeePlayer.png");
        gameObject->AddTag("Enemy");
    }


    auto* health = gameObject->AddComponent<HealthComponent>(4);
    health->SetSpawnCell({ col, row });
    int savedLives = PlayerManager::GetInstance().GetLives(1);
    if (savedLives > 0) health->SetLives(savedLives);


    m_PlayerObjects["Pengo2"] = gameObject.get();
    PlayerManager::GetInstance().RegisterPlayer(1, gameObject.get());

    return gameObject;
}