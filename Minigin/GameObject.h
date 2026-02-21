#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "GameComponent.h"
#include <type_traits>

namespace dae
{
	class Texture2D;
	class GameComponent;

	class GameObject final
	{
		Transform m_transform{};
	public:
		void Update();
		void Render() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		Transform& GetTransform() { return m_transform; };
		void SetPosition(float x, float y);
		
		//Marking for destruction
		void MarkForDestruction();
		
		bool isMarkedForDestruction() { return m_MarkForDestruction; }

		//Parenting
		void SetParent(GameObject* pParent, bool keepWorldPosition = true);
		
		GameObject* GetParent() const { return m_pParent; }
		GameObject* GetChildAt(int index) const { return m_pChildren[index]; }
		int GetChildCount() const{ return static_cast<int>(m_pChildren.size()); }

		//Dirty flag
		void SetLocalPosition(const glm::vec3& pos);
		void SetPositionDirty();
		void UpdateWorldPosition();

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }

#pragma region ComponentTemplates
		//Component
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<GameComponent, T>::value, "T does not inherit from GameComponent");

			auto pComponent{ std::make_unique<T>(this, std::forward<Args>(args)...) };

			T* ptr = pComponent.get();
			m_pComponents.push_back(std::move(pComponent));
			
			return ptr;
		}

		template<typename T>
		void RemoveComponent()
		{
			for (auto i = m_pComponents.begin(); i < m_pComponents.end(); i++)
			{
				if (dynamic_cast<T*>(i->get()))
				{
					m_pComponents.erase(i);
					return;
				}
			}
		}

		template<typename T>
		T* GetComponent() const
		{
			for (const auto& pComp : m_pComponents)
			{
				if (pComp)
				{
					if (auto component = dynamic_cast<T*>(pComp.get()))
					{
						return component;
					}
				}
			}
			return nullptr;
		}

		template<typename T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}
#pragma endregion
		
	private:

		//Parenting
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);
		bool IsChildOf(GameObject* pObject) const;
		
		//Components
		std::vector<std::unique_ptr<GameComponent>> m_pComponents{};

		//Marking for destruction
		bool m_MarkForDestruction{ false };

		//Parenting
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;

		//Dirty flag
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_PositionIsDirty{ true };
	};
}
