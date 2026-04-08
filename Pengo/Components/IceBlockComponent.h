#pragma once
#include "GameComponent.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "Components/CollisionComponent.h"

namespace dae
{
    class IceBlockComponent final : public GameComponent
    {
    public:
        IceBlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row);
        virtual ~IceBlockComponent();

        IceBlockComponent(const IceBlockComponent&) = delete;
        IceBlockComponent(IceBlockComponent&&) = delete;
        IceBlockComponent& operator=(const IceBlockComponent&) = delete;
        IceBlockComponent& operator=(IceBlockComponent&&) = delete;

        void Update() override;
        void Push(glm::ivec2 direction, GameObject* player);
        void Break();

        glm::ivec2 GetCell() const { return m_Cell; }
        GridComponent* GetGrid() const { return m_pGrid; }

    private:
        GridComponent* m_pGrid{ nullptr };
        glm::ivec2 m_Cell{};


        GridMoveComponent* m_pMoveComponent;
        CollisionComponent* m_pCollisionComponent;

        glm::ivec2 m_SlideDirection{ 0, 0 };
        bool m_IsSliding{ false };

        GameObject* m_pPlayer{ nullptr };
    };
}