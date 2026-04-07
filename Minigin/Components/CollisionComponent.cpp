#include "CollisionComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Transform.h"
#include <SDBMHasher.h>
#include "Events/EventManager.h"

dae::CollisionComponent::CollisionComponent(GameObject* gameObject)
	:GameComponent(gameObject)
{
	m_Transform = gameObject->GetComponent<Transform>();
	
	CollisionManager::GetInstance().AddCollider(this);
}

dae::CollisionComponent::~CollisionComponent()
{
	CollisionManager::GetInstance().RemoveCollider(this);
}

void dae::CollisionComponent::Update()
{
	CollisionManager::GetInstance().CheckCollision(this);
}

void dae::CollisionComponent::SetSize(int width, int height)
{
	m_Size.x = static_cast<float>(width);
	m_Size.y = static_cast<float>(height);
}

void dae::CollisionComponent::OnCollision(GameObject* other)
{
	//Quick Tag checking --> Should be in Game not Engine
	if (other->HasTag("Enemy"))
	{
		Event e(make_sdbm_hash("PlayerHit"));
		e.nbArgs = 1;
		e.args[0].gameObject = GetGameObject();
		EventManager::GetInstance().HandleEvent(e);
	}
}
