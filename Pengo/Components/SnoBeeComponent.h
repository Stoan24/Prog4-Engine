#pragma once
#include "GameComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "Observers/Observer.h"
#include "Observers/Subject.h"
#include <glm/fwd.hpp>
#include <SDBMHasher.h>

namespace dae
{
    enum class SnoBeeState
    {
        Wander,
        Chase, //Maybe add chasing later
        Stunned
    };

    class SnoBeeComponent final : public GameComponent, public Observer
    {
    public:
        SnoBeeComponent(GameObject* gameObject, GridComponent* grid);
        virtual ~SnoBeeComponent() = default;

        SnoBeeComponent(const SnoBeeComponent&) = delete;
        SnoBeeComponent(SnoBeeComponent&&) = delete;
        SnoBeeComponent& operator=(const SnoBeeComponent&) = delete;
        SnoBeeComponent& operator=(SnoBeeComponent&&) = delete;

        void Update() override;

        void Stun(float duration);
        bool IsStunned() const { return m_State == SnoBeeState::Stunned; }

        void Notify(const Event& e) override
        {
            if (e.id == make_sdbm_hash("StunEnemies"))
            {
                Stun(2);
            }
        }

    private:
        GridComponent* m_pGrid{ nullptr };
        GridMoveComponent* m_pMove{ nullptr };

        SnoBeeState m_State{ SnoBeeState::Wander };

        float m_StunTimer{ 0.f };

        float m_BlockBreakTimer{ 0.f };
        const float m_BlockBreakCooldown{ 1.5f };

        glm::ivec2 m_WanderTarget{ -1, -1 };

        std::unique_ptr<Subject> m_pSubject;

        void UpdateWander();

        bool BreakBlock(glm::ivec2 direction);

        glm::ivec2 PickRandomCell() const;
    };
}