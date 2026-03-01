#pragma once
#include <glm/glm.hpp>
#include "GameComponent.h"

namespace dae
{
	class Transform final : public GameComponent
	{
	public:
		Transform(dae::GameObject* pGameObject)
			:GameComponent(pGameObject)
		{
		}

		virtual ~Transform() = default;

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		
		void SetWorldPosition(float x, float y, float z = 0);
		void SetLocalPosition(float x, float y, float z = 0);

		void SetWorldPosition(const glm::vec3& position);
		void SetLocalPosition(const glm::vec3& position);

		void SetPositionDirty();
		void UpdateWorldPosition();


	private:
		glm::vec3 m_WorldPosition{ 0, 0, 0 };
		glm::vec3 m_LocalPosition{ 0, 0, 0 };

		bool m_IsPositionDirty{ true };
	};
}
