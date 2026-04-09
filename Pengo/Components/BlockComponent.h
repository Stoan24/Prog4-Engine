#pragma once
#include "GameComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "Components/CollisionComponent.h"

namespace dae {
    class BlockComponent : public GameComponent {
    public:
        BlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row);

        virtual ~BlockComponent() = default;

        void Update();

        void Push(glm::ivec2 direction, GameObject* player);
        

    protected:

        virtual void OnBreak() = 0;
        virtual void OnSlideStopped() {}

        GridComponent* m_pGrid;
        GridMoveComponent* m_pMoveComponent;

        CollisionComponent* m_pCollisionComponent;

        glm::ivec2 m_SlideDirection{ 0, 0 };
        bool m_IsSliding{ false };
        GameObject* m_pPlayer{ nullptr };
    };
}