#pragma once
#include "GameComponent.h"
#include "GameObject.h"
#include "Components/GridComponent.h"
#include "Components/GridMoveComponent.h"
#include "Components/CollisionComponent.h"

namespace dae {
    class BlockComponent : public GameComponent {
    public:
        BlockComponent(GameObject* gameObject, GridComponent* grid);

        virtual ~BlockComponent() = default;

        void Update();

        void Push(glm::ivec2 direction, GameObject* player);
        
        virtual void OnBreak(bool playsSound) = 0;

    protected:

        virtual void OnSlideStopped() {}

        GridComponent* m_pGrid;
        GridMoveComponent* m_pMoveComponent;

        CollisionComponent* m_pCollisionComponent;

        glm::ivec2 m_SlideDirection{ 0, 0 };
        bool m_IsSliding{ false };
        GameObject* m_pPlayer{ nullptr };

        int m_KillCount{ 0 };

        static constexpr int m_KillScores[] = { 0, 400, 1600, 3200, 6400 };
        static constexpr int m_MaxKills = 4;
    };
}