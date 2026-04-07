#include "GridComponent.h"

dae::GridComponent::GridComponent(GameObject* gameObject, int cols, int rows, int cellSize, glm::vec2 origin)
    : GameComponent(gameObject),
    m_Cols{ cols },
    m_Rows{ rows },
    m_CellSize{ cellSize },
    m_Origin{ origin }
{
}

glm::vec2 dae::GridComponent::CellToWorld(int col, int row) const
{
    float x = m_Origin.x + col * m_CellSize + m_CellSize * 0.5f;
    float y = m_Origin.y + row * m_CellSize + m_CellSize * 0.5f;

    return glm::vec2{ x, y };
}

glm::vec2 dae::GridComponent::CellToWorld(glm::ivec2 cell) const
{
    return CellToWorld(cell.x, cell.y);
}

glm::ivec2 dae::GridComponent::WorldToCell(glm::vec2 worldPos) const
{
    int x = static_cast<int>((worldPos.x - m_Origin.x) / m_CellSize);
    int y = static_cast<int>((worldPos.y - m_Origin.y) / m_CellSize);

    return glm::ivec2{ x, y };
}

bool dae::GridComponent::IsValidCell(int col, int row) const
{
    return col >= 0 && col < m_Cols && row >= 0 && row < m_Rows;
}

bool dae::GridComponent::IsValidCell(glm::ivec2 cell) const
{
    return IsValidCell(cell.x, cell.y);
}

void dae::GridComponent::FillCell(int col, int row, GameObject* occupant)
{
    m_OccupiedCells[CellKey(col, row)] = occupant;
}

void dae::GridComponent::FreeCell(int col, int row)
{
    m_OccupiedCells.erase(CellKey(col, row));
}

bool dae::GridComponent::IsCellFree(int col, int row) const
{
    return m_OccupiedCells.find(CellKey(col, row)) == m_OccupiedCells.end();
}

dae::GameObject* dae::GridComponent::GetCellObject(int col, int row) const
{
    auto it = m_OccupiedCells.find(CellKey(col, row));
    if (it != m_OccupiedCells.end()) return it->second;
    return nullptr;
}