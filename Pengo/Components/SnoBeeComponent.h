#pragma once
#include "GameComponent.h"
#include "GameObject.h"

#include "Observers/Observer.h"
#include "Observers/Subject.h"

#include "GridComponent.h"
#include "GridMoveComponent.h"

#include "SDBMHasher.h"

namespace dae
{
    enum class SnoBeeState
    {
        Wander,
        Chase, //Maybe add chasing later
        Stunned
    };

    class SnoBeeComponent final : public GameComponent, public IObserver
    {
    public:
        SnoBeeComponent(GameObject* gameObject, GridComponent* grid);
        virtual ~SnoBeeComponent();

        SnoBeeComponent(const SnoBeeComponent&) = delete;
        SnoBeeComponent(SnoBeeComponent&&) = delete;
        SnoBeeComponent& operator=(const SnoBeeComponent&) = delete;
        SnoBeeComponent& operator=(SnoBeeComponent&&) = delete;

        void FixedUpdate() override;

        void Stun(float duration);
        bool IsStunned() const { return m_State == SnoBeeState::Stunned; }

        void Notify(const Event& e) override;
        

    private:
        GridComponent* m_pGrid{ nullptr };
        GridMoveComponent* m_pMove{ nullptr };

        SnoBeeState m_State{ SnoBeeState::Wander };

        float m_StunTimer{ 0.f };
        float m_maxStun{ 7.5f };

        float m_BlockBreakTimer{ 0.f };
        const float m_BlockBreakCooldown{ 1.5f };

        glm::ivec2 m_WanderTarget{ -1, -1 };

        void UpdateWander();

        bool BreakBlock(glm::ivec2 direction);

        glm::ivec2 PickRandomCell() const;
    };
}