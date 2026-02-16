#pragma once

namespace dae
{
	class GameObject;

	class GameComponent
	{
	public:
		GameComponent(GameObject* pGameObject) : m_pGameObject(pGameObject) {}
		
		virtual ~GameComponent() = default;
		GameComponent(const GameComponent& other) = default;
		GameComponent(GameComponent&& other) = default;
		GameComponent& operator=(const GameComponent& other) = default;
		GameComponent& operator=(GameComponent&& other) = default;

		virtual void Update() {}
		virtual void Render() {}


		virutal GameObject GetGameObject() const {return m_pGameObject}

	private:
		GameObject* m_pGameObject;
	};
}