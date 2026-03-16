#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include "Transform.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"

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
        GameObject* m_pGameObject;
        glm::vec2 m_Direction;
        float m_MoveSpeed;

        Transform* m_Transform;

    public:
        MoveCommand(GameObject* gameObject, glm::vec2 direction, float speed)
            :m_pGameObject{ gameObject },
            m_Direction{ direction },
            m_MoveSpeed{ speed }
        {
            m_Transform = m_pGameObject->GetComponent<Transform>();
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

    class ScoreCommand final : public Command
    {
        GameObject* m_pGameObject;

    public:
        ScoreCommand(GameObject* gameObject)
            :m_pGameObject{ gameObject }
        {
        }

        void Execute() override
        {
            Event e(make_sdbm_hash("AddScore"));

            e.nbArgs = 2;
            e.args[0].gameObject = m_pGameObject;
            e.args[1].value = 100;

            EventManager::GetInstance().HandleEvent(e);
        }
    };
}