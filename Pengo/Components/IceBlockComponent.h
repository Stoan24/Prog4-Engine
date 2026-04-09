#pragma once
#include "BlockComponent.h"
#include "GameObject.h"
#include "GridComponent.h"

namespace dae
{
    class IceBlockComponent final : public BlockComponent
    {
    public:
        IceBlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row);

        virtual ~IceBlockComponent();

        IceBlockComponent(const IceBlockComponent&) = delete;
        IceBlockComponent(IceBlockComponent&&) = delete;
        IceBlockComponent& operator=(const IceBlockComponent&) = delete;
        IceBlockComponent& operator=(IceBlockComponent&&) = delete;

    protected:
        
        void OnBreak() override;
    };
}