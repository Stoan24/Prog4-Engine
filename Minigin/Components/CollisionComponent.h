#pragma once
#include "GameComponent.h"
#include <glm/vec2.hpp>

namespace dae
{
	class Transform;
	class Subject;
	class CollisionComponent final : public GameComponent
	{
	public:
		explicit CollisionComponent(GameObject* gameObject);
		virtual ~CollisionComponent();

		void Update() override;

		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;

		void SetSize(glm::vec2 size) { m_Size = size; };
		void SetSize(int width, int height);
		glm::vec2 GetSize() const { return m_Size; };

		Transform* GetTransform() { return m_Transform; };

		GameObject* GetGameObject() const override { return GameComponent::GetGameObject(); }


		void OnCollision(GameObject* other);

	private:
		
		Transform* m_Transform;
		glm::vec2 m_Size{};
	};
}