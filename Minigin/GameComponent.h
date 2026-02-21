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

	protected:
		GameComponent(GameObject* pGameObject) : m_pGameObject(pGameObject) {}
		virtual GameObject* GetGameObject() const { return m_pGameObject;  }

	private:
		GameObject* m_pGameObject;
	};
}