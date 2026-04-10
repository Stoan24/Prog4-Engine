#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>

void dae::SceneManager::Update()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Update();
	}
}

void dae::SceneManager::Render()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Render();
	}
}

void dae::SceneManager::RenderGUI()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->RenderGUI();
	}
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const auto& s)
		{
			return s->GetName() == name;
		});

	if (it != m_scenes.end())
	{
		m_pActiveScene = it->get();
	}
}

dae::Scene& dae::SceneManager::GetActiveScene() const
{
	return *m_pActiveScene;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	m_scenes.emplace_back(new Scene(name));
	return *m_scenes.back();
}
