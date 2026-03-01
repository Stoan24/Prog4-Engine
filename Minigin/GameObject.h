#pragma once
#include <string>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <utility>
#include "GameComponent.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class GameComponent;

	class GameObject final
	{
	public:
		void Update();
		void Render() const;

		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		//Marking for destruction
		void MarkForDestruction();
		
		bool isMarkedForDestruction() const { return m_IsMarkForDestruction; }
		void MarkToDestroy();

		//Parenting
		void SetParent(GameObject* pParent, bool keepWorldPosition = true);
		
		GameObject* GetParent() const { return m_pParent; }
		GameObject* GetChildAt(int index) const { return m_pChildren[index]; }
		int GetChildCount() const{ return static_cast<int>(m_pChildren.size()); }
		std::vector<GameObject*> GetChildren() const;

#pragma region ComponentTemplates
		//Component
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<GameComponent, T>::value, "T does not inherit from GameComponent");

			auto pComponent{ std::make_unique<T>(this, std::forward<Args>(args)...) };
			

			auto ptr = pComponent.get();
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

		void DestroyMarkedComponents();
		
		//Components
		std::vector<std::unique_ptr<GameComponent>> m_pComponents{};

		//Marking for destruction
		bool m_IsMarkForDestruction{ false };

		//Parenting
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;
	};
}
