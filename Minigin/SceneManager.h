#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();
		void RenderGUI();

		void Clear() { m_scenes.clear(); m_scenes.shrink_to_fit(); m_pActiveScene = nullptr; };


		void SetActiveScene(const std::string& name);
		Scene& GetActiveScene() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		virtual ~SceneManager() = default;

		std::vector<std::unique_ptr<Scene>> m_scenes{};

		Scene* m_pActiveScene{ nullptr };
	};
}
