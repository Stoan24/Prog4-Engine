#pragma once
#include "GameComponent.h"
#include "Transform.h"

namespace dae
{
	class RotationComponent final : public GameComponent
	{
	public:
		explicit RotationComponent(GameObject* pGameObject, float radius, float speed);

		~RotationComponent() override = default;

		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;

		void Update() override;

	private:
		float m_angle;
		float m_radius;
		float m_speed;

		glm::vec3 m_offset{};
	};
}