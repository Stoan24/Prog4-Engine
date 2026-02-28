#include "Transform.h"
#include "GameObject.h"

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_isPositionDirty)
	{
		UpdateWorldPosition();
	}
	return m_worldposition;
}

void dae::Transform::SetWorldPosition(const float x, const float y, const float z)
{
	m_worldposition.x = x;
	m_worldposition.y = y;
	m_worldposition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetWorldPosition(const glm::vec3& position) 
{ 
	m_worldposition = position;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_localPosition.x = x;
	m_localPosition.y = y;
	m_localPosition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_localPosition = position;
	SetPositionDirty();
}

void dae::Transform::SetPositionDirty()
{
	m_isPositionDirty = true;
}

void dae::Transform::UpdateWorldPosition()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		m_worldposition = m_localPosition;
	}
	else
	{
		const auto parentTransform = GetGameObject()->GetParent()->GetComponent<Transform>();
		const auto parentWorldPosition = parentTransform->GetWorldPosition();
		m_worldposition = parentWorldPosition + m_localPosition;
	}

	m_isPositionDirty = false;
}
