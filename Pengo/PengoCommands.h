#pragma once
#include "Commands.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Events/EventManager.h"
#include "SDBMHasher.h"
#include <Components/GridMoveComponent.h>

namespace dae
{
    class MoveCommand final : public Command
    {
        GameObject* m_pGameObject;
        glm::ivec2 m_Direction;

    public:
        MoveCommand(GameObject* gameObject, glm::ivec2 direction)
            :m_pGameObject{ gameObject },
            m_Direction{ direction }
        {
        }

        void Execute() override
        {
            auto* gridMove = m_pGameObject->GetComponent<GridMovementComponent>();
            if (gridMove && !gridMove->IsMoving())
            {
                gridMove->TryMove(m_Direction);
            }
        }
    };

    class KillEnemyCommand final : public Command
    {
        GameObject* m_pGameObject;

    public:
        KillEnemyCommand(GameObject* gameObject)
            : m_pGameObject{ gameObject }
        {
        }

        void Execute() override
        {
            Event e(make_sdbm_hash("EnemyKilled"));
            e.nbArgs = 1;
            e.args[0].gameObject = m_pGameObject;
            EventManager::GetInstance().HandleEvent(e);
        }
    };

    class PushBlockCommand final : public Command
    {
        GameObject* m_pGameObject;

    public:
        PushBlockCommand(GameObject* gameObject)
            : m_pGameObject{ gameObject }
        {
        }

        void Execute() override
        {
            Event e(make_sdbm_hash("BlockPushed"));
            e.nbArgs = 1;
            e.args[0].gameObject = m_pGameObject;
            EventManager::GetInstance().HandleEvent(e);
        }
    };
}