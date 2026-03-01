#include "Transform.h"
#include "GameObject.h"

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void dae::Transform::SetWorldPosition(const float x, const float y, const float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetWorldPosition(const glm::vec3& position) 
{ 
	m_WorldPosition = position;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetPositionDirty();
}

void dae::Transform::SetPositionDirty()
{
	m_IsPositionDirty = true;

	for (int i = 0; i < GetGameObject()->GetChildCount(); i++)
	{
		GetGameObject()->GetChildAt(i)->GetComponent<Transform>()->SetPositionDirty();
	}
}

void dae::Transform::UpdateWorldPosition()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
	}
	else
	{
		const auto parentTransform = GetGameObject()->GetParent()->GetComponent<Transform>();
		const auto parentWorldPosition = parentTransform->GetWorldPosition();
		m_WorldPosition = parentWorldPosition + m_LocalPosition;
	}

	m_IsPositionDirty = false;
}
