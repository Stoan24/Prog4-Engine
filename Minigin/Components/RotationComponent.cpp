#include "RotationComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "math.h"

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
	const double pi = 2 * acos(0.0);
	const float twoPi = static_cast<float>(pi) * 2.f;

	m_angle += m_speed * deltaTime;

	//Reset
	if (m_angle >= twoPi) m_angle -= twoPi;
	else if (m_angle < 0) m_angle += twoPi;

	float x = std::cos(m_angle) * m_radius + m_offset.x;
	float y = std::sin(m_angle) * m_radius + m_offset.y;

	GetGameObject()->GetComponent<Transform>()->SetLocalPosition(glm::vec3(x, y, 0.0f));
}
