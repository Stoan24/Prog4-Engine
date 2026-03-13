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


    class MoveCommand final : public Command
    {
        GameObject* m_pActor;
        glm::vec2 m_Direction;
        float m_MoveSpeed;

        Transform* m_Transform;

    public:
        MoveCommand(GameObject* actor, glm::vec2 direction, float speed)
            : m_pActor(actor), m_Direction(direction), m_MoveSpeed(speed)
        {
            m_Transform = m_pActor->GetComponent<Transform>();
        }

        void Execute() override
        {
            if (!m_Transform) return;

            const float deltaTime = GameTime::GetInstance().GetDeltaTime();

            auto pos = m_Transform->GetLocalPosition();

            pos.x += m_Direction.x * m_MoveSpeed * deltaTime;
            pos.y += m_Direction.y * m_MoveSpeed * deltaTime;

            m_Transform->SetLocalPosition(pos.x, pos.y);
        }
    };
}