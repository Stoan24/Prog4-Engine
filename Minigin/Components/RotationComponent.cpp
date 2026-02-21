#include "RotationComponent.h"
#include "GameObject.h"
#include "GameTime.h"

dae::RotationComponent::RotationComponent(GameObject* pGameObject, float radius, float speed)
	:GameComponent{ pGameObject },
	m_angle{0.f},
	m_radius{radius},
	m_speed{speed}
{
}

dae::RotationComponent::RotationComponent(GameObject* pGameObject, float radius, float speed, const glm::vec3& offset)
	:RotationComponent(pGameObject, radius, speed)
{
	m_offset = offset;
}

void dae::RotationComponent::Update()
{
	const float deltaTime = GameTime::GetInstance().GetDeltaTime();

	m_angle += m_speed * deltaTime;

	float x = std::cos(m_angle) * m_radius + m_offset.x;
	float y = std::sin(m_angle) * m_radius + m_offset.y;

	GetGameObject()->SetLocalPosition(glm::vec3(x, y, 0.0f));
}
