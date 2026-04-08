#pragma once
#include "GameComponent.h"
#include "GameObject.h"
#include <glm/vec2.hpp>
#include <unordered_map>

namespace dae
{
    class GridComponent final : public GameComponent
    {
    public:
        GridComponent(GameObject* gameObject, int cols = 13, int rows = 15, int cellSize = 16, glm::vec2 origin = { 0.f, 0.f });
        virtual ~GridComponent() = default;

        GridComponent(const GridComponent&) = delete;
        GridComponent(GridComponent&&) = delete;
        GridComponent& operator=(const GridComponent&) = delete;
        GridComponent& operator=(GridComponent&&) = delete;


        glm::vec2 CellToWorld(int col, int row) const;
        glm::vec2 CellToWorld(glm::ivec2 cell) const;

        glm::ivec2 WorldToCell(glm::vec2 worldPos) const;


        bool IsValidCell(int col, int row) const;
        bool IsValidCell(glm::ivec2 cell) const;


        int GetCols() const { return m_Cols; }
        int GetRows() const { return m_Rows; }
        int GetCellSize() const { return m_CellSize; }
        glm::vec2 GetOrigin() const { return m_Origin; }


        //Tracking what is inside the cells
        void FillCell(int col, int row, GameObject* gameObject);
        void FreeCell(int col, int row);
        bool IsCellFree(int col, int row) const;
        GameObject* GetCellObject(int col, int row) const;

        GameObject* GetGameObject() const override { return GameComponent::GetGameObject(); }

    private:
        int m_Cols{ 13 };
        int m_Rows{ 15 };
        int m_CellSize{ 16 };
        glm::vec2 m_Origin{ 0.f, 0.f };


        std::unordered_map<int, GameObject*> m_OccupiedCells;
        int CellKey(int col, int row) const { return row * m_Cols + col; }
    };
}