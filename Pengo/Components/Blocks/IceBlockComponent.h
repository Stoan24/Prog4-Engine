#pragma once
#include "BlockComponent.h"
#include "GameObject.h"
#include "Components/GridComponent.h"

namespace dae
{
    class IceBlockComponent final : public BlockComponent
    {
    public:
        IceBlockComponent(GameObject* gameObject, GridComponent* grid);

        virtual ~IceBlockComponent();

        IceBlockComponent(const IceBlockComponent&) = delete;
        IceBlockComponent(IceBlockComponent&&) = delete;
        IceBlockComponent& operator=(const IceBlockComponent&) = delete;
        IceBlockComponent& operator=(IceBlockComponent&&) = delete;
        
        void OnBreak() override;
    };
}