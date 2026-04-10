#pragma once
#include "BlockComponent.h"
#include "Components/GridComponent.h"

namespace dae
{
    class EggBlockComponent final : public BlockComponent
    {
    public:
        EggBlockComponent(GameObject* gameObject, GridComponent* grid);
        virtual ~EggBlockComponent() = default;

        EggBlockComponent(const EggBlockComponent&) = delete;
        EggBlockComponent(EggBlockComponent&&) = delete;
        EggBlockComponent& operator=(const EggBlockComponent&) = delete;
        EggBlockComponent& operator=(EggBlockComponent&&) = delete;

        void Hatch();

        bool HasHatched() const { return m_HasHatched; }

    protected:

        void OnBreak() override;
        void OnSlideStopped() override {}

    private:
        void Destroy();

        bool m_HasHatched{ false };
    };
}