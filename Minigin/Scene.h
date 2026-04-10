#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"
#include "Observers/Observer.h"

namespace dae
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& name)
			: m_name(name) 
		{
		}

		void Add(std::unique_ptr<GameObject> object);
		void Add(std::unique_ptr<Observer> observer);
		void Remove(const GameObject& object);
		void Remove(const Observer& observer);

		void Update();
		void Render() const;
		void RenderGUI();


		const std::string& GetName() const { return m_name; }
		const std::vector<std::unique_ptr<GameObject>>& GetObjects() const { return m_objects; }

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;

		std::string m_name;

		std::vector <std::unique_ptr<GameObject>> m_objects{};
		std::vector <std::unique_ptr<Observer>> m_observers{};
	};

}
