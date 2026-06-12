#pragma once
#include "Commands.h"
#include "GameObject.h"
#include "Components/GridMoveComponent.h"
#include "Components/Blocks/BlockComponent.h"
#include "Components/SnoBeeComponent.h"

namespace dae
{
    class MoveCommand final : public Command
    {
        GameObject* m_pGameObject;
        glm::ivec2 m_Direction;

        GridMoveComponent* m_MoveComponent;

    public:
        MoveCommand(GameObject* gameObject, glm::ivec2 direction)
            :m_pGameObject{ gameObject },
            m_Direction{ direction }
        {
            m_MoveComponent = m_pGameObject->GetComponent<GridMoveComponent>();
        }

        void Execute() override
        {
            if (m_MoveComponent && !m_MoveComponent->IsMoving())
            {
                m_MoveComponent->Move(m_Direction, false);
            }
        }
    };

    class PushBlockCommand final : public Command
    {
        GameObject* m_pGameObject;

        GridMoveComponent* m_MoveComponent;
        GridComponent* m_Grid;

    public:
        PushBlockCommand(GameObject* gameObject)
            : m_pGameObject{ gameObject }
        {
            m_MoveComponent = m_pGameObject->GetComponent<GridMoveComponent>();

            m_Grid = m_pGameObject->GetComponent<GridMoveComponent>()->GetGrid();
        }

        void Execute() override
        {
            if (!m_MoveComponent || !m_Grid) return;


            glm::ivec2 lookDir = m_MoveComponent->GetLookDirection();
            glm::ivec2 targetCell = m_MoveComponent->GetCurrentCell() + lookDir;

            if (auto* snoBee = m_pGameObject->GetComponent<SnoBeeComponent>())
            {
                snoBee->TryManualBlockBreak(lookDir);
                return;
            }

            GameObject* targetObject = m_Grid->GetCellObject(targetCell.x, targetCell.y);
            if (targetObject)
            {
                auto* iceBlock = targetObject->GetComponent<BlockComponent>();
                if (iceBlock)
                {
                    iceBlock->Push(lookDir, m_pGameObject);
                }
            }
        }
    };
}