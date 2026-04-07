#pragma once
#include "GameComponent.h"
#include "Components/GridComponent.h"

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

        glm::ivec2 GetCell() const { return m_Cell; }
        GridComponent* GetGrid() const { return m_Grid; }

    private:
        GridComponent* m_Grid{ nullptr };
        glm::ivec2 m_Cell{};
    };
}