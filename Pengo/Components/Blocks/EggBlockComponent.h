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

        void SetFlashing(bool flashing);
        void UpdateFlash();

    protected:

        void OnBreak(bool playsSound) override;
        void OnSlideStopped() override {}

    private:
        void Destroy();

        bool m_HasHatched{ false };

        bool m_IsFlashing{ false };
        float m_FlashTimer{ 0.f };
        const float m_FlashInterval{ 0.4f };
        bool m_FlashToggle{ false };
        const std::string m_NormalTexture{ "IceBlock.png" };
        const std::string m_FlashTexture{ "FlashBlock.png" };

        int m_eggBreakScore{ 500 };
    };
}