#pragma once
#include "GameComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include <glm/glm.hpp>

namespace dae
{
    class GridMovementComponent final : public GameComponent
    {
    public:
        GridMovementComponent(GameObject* gameObject, GridComponent* grid, int startCol, int startRow, float moveSpeed);
        virtual ~GridMovementComponent() = default;

        GridMovementComponent(const GridMovementComponent&) = delete;
        GridMovementComponent(GridMovementComponent&&) = delete;
        GridMovementComponent& operator=(const GridMovementComponent&) = delete;
        GridMovementComponent& operator=(GridMovementComponent&&) = delete;

        void Update() override;


        bool TryMove(glm::ivec2 direction);


        bool IsMoving() const { return m_IsMoving; }
        glm::ivec2 GetCurrentCell() const { return m_CurrentCell; }

    private:
        GridComponent* m_Grid{ nullptr };

        glm::ivec2 m_CurrentCell{};
        glm::ivec2 m_TargetCell{};
        glm::vec2  m_StartWorldPos{};
        float m_MoveProgress{ 0.f };
        float m_MoveSpeed{ 5.f };
        bool m_IsMoving{ false };
    };
}