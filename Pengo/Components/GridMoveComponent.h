#pragma once
#include "GameComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include <glm/glm.hpp>

namespace dae
{
    class GridMoveComponent final : public GameComponent
    {
    public:
        GridMoveComponent(GameObject* gameObject, GridComponent* grid, int startCol, int startRow, float moveSpeed);
        virtual ~GridMoveComponent() = default;

        GridMoveComponent(const GridMoveComponent&) = delete;
        GridMoveComponent(GridMoveComponent&&) = delete;
        GridMoveComponent& operator=(const GridMoveComponent&) = delete;
        GridMoveComponent& operator=(GridMoveComponent&&) = delete;

        void Update() override;


        bool Move(glm::ivec2 direction, bool isBlock);
        glm::ivec2 GetLookDirection();

        GridComponent* GetGrid() const { return m_pGrid; }

        bool IsMoving() const { return m_IsMoving; }
        glm::ivec2 GetCurrentCell() const { return m_CurrentCell; }

    private:
        GridComponent* m_pGrid{ nullptr };

        glm::ivec2 m_CurrentCell{};
        glm::ivec2 m_TargetCell{};
        glm::vec2  m_StartWorldPos{};
        float m_MoveProgress{ 0.f };
        float m_MoveSpeed{ 5.f };
        bool m_IsMoving{ false };

        glm::ivec2 m_LookDirection{ 0, 0 };
    };
}