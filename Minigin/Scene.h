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
		void Add(std::unique_ptr<IObserver> observer);
		void Remove(const GameObject& object);
		void Remove(const IObserver& observer);

		void Update();
		void FixedUpdate();
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
		std::vector <std::unique_ptr<IObserver>> m_observers{};
	};

}
