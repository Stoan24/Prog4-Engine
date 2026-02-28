#pragma once
#include <glm/glm.hpp>
#include "GameComponent.h"

namespace dae
{
	class Transform final : public GameComponent
	{
	public:
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_localPosition; }
		
		void SetWorldPosition(float x, float y, float z = 0);
		void SetLocalPosition(float x, float y, float z = 0);

		void SetWorldPosition(const glm::vec3& position);
		void SetLocalPosition(const glm::vec3& position);

		void SetPositionDirty();
		void UpdateWorldPosition();


	private:
		glm::vec3 m_worldposition{ 0, 0, 0 };
		glm::vec3 m_localPosition{ 0, 0, 0 };

		bool m_isPositionDirty{ true };
	};
}
