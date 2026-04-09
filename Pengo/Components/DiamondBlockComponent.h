#pragma once
#include "BlockComponent.h"
#include "GameObject.h"
#include "GridComponent.h"

namespace dae
{
    class DiamondBlockComponent final : public BlockComponent
    {
    public:
        DiamondBlockComponent(GameObject* gameObject, GridComponent* grid, int col, int row);

        virtual ~DiamondBlockComponent() = default;

        DiamondBlockComponent(const DiamondBlockComponent&) = delete;
        DiamondBlockComponent(DiamondBlockComponent&&) = delete;
        DiamondBlockComponent& operator=(const DiamondBlockComponent&) = delete;
        DiamondBlockComponent& operator=(DiamondBlockComponent&&) = delete;

    protected:

        void OnBreak() override 
        {
            //Cannot break
        }

        void OnSlideStopped() override 
        {
            CheckAlignment();
        }

    private:

        void CheckAlignment();
    };
}