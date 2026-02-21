#pragma once
#include "GameComponent.h"
#include "Transform.h"

namespace dae
{
	class RotationComponent final : public GameComponent
	{
	public:
		explicit RotationComponent(GameObject* pGameObject, float radius, float speed);
		explicit RotationComponent(GameObject* pGameObject, float radius, float speed, const glm::vec3& offset);

		~RotationComponent() override = default;

		void Update() override;

	private:
		float m_angle;
		float m_radius;
		float m_speed;

		glm::vec3 m_offset{};
	};
}