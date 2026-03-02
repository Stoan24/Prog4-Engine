#pragma once

namespace dae
{
	class GameObject;

	class GameComponent
	{
	public:
		virtual ~GameComponent() = default;
		GameComponent(const GameComponent& other) = delete;
		GameComponent(GameComponent&& other) = delete;
		GameComponent& operator=(const GameComponent& other) = delete;
		GameComponent& operator=(GameComponent&& other) = delete;
		
		virtual void Update() {}
		virtual void Render() const {}
		virtual void RenderGUI() {}

		bool IsMarkedForDestruction() const { return m_IsMarkedForDestruction; }
		void MarkToDestroy() { m_IsMarkedForDestruction = true; }

	protected:
		GameComponent(GameObject* pGameObject) : m_pGameObject(pGameObject) {}
		virtual GameObject* GetGameObject() const { return m_pGameObject;  }

	private:
		GameObject* m_pGameObject;

		bool m_IsMarkedForDestruction{ false };
	};
}