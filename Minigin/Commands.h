#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include "Transform.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};


    class MoveCommand final : public dae::Command
    {
        dae::GameObject* m_pActor;
        glm::vec2 m_Direction;
        float m_MoveSpeed;

    public:
        MoveCommand(dae::GameObject* actor, glm::vec2 direction, float speed)
            : m_pActor(actor), m_Direction(direction), m_MoveSpeed(speed)
        {
        }

        void Execute() override
        {
            const float deltaTime = GameTime::GetInstance().GetDeltaTime();

            auto transform = m_pActor->GetComponent<dae::Transform>();
            auto pos = transform->GetLocalPosition();

            pos.x += m_Direction.x * m_MoveSpeed * deltaTime;
            pos.y += m_Direction.y * m_MoveSpeed * deltaTime;

            transform->SetLocalPosition(pos.x, pos.y);
        }
    };
}