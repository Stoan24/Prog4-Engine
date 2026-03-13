#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"
#include "Observer.h"

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Add(std::unique_ptr<Observer> observer);
		void Remove(const GameObject& object);
		void Remove(const Observer& observer);

		void Update();
		void Render() const;
		void RenderGUI();

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::vector <std::unique_ptr<GameObject>> m_objects{};
		std::vector <std::unique_ptr<Observer>> m_observers{};
	};

}
