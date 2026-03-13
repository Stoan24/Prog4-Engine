#include <algorithm>
#include "Scene.h"
#include <assert.h>

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Add(std::unique_ptr<Observer> observer)
{
	assert(observer != nullptr && "Cannot add a null Observer to the scene.");
	m_observers.emplace_back(std::move(observer));
}

void Scene::Remove(const GameObject& object)
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_objects.end()
	);
}

void dae::Scene::Remove(const Observer& observer)
{
	m_observers.erase(
		std::remove_if(
			m_observers.begin(),
			m_observers.end(),
			[&observer](const auto& ptr) { return ptr.get() == &observer; }
		),
		m_observers.end()
	);
}

void Scene::Update()
{
	//Update
	for(auto& object : m_objects)
	{
		object->Update();
	}

	//Removal
	std::erase_if(m_objects, [](const std::unique_ptr<GameObject>& object) 
		{
			return object->isMarkedForDestruction();
		});

}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::RenderGUI()
{
	for (const auto& object : m_objects)
	{
		object->RenderGUI();
	}
}

