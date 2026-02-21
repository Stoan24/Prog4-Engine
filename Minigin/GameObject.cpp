#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (auto& component : m_pComponents)
	{
		component->Update();
	}
}

void dae::GameObject::Render() const
{
	for(auto& component : m_pComponents)
	{
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::MarkForDestruction()
{
	m_MarkForDestruction = true;
	
	for(auto child : m_pChildren)
	{
		child->MarkForDestruction();
	}
}

#pragma region DirtyFlag
void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = m_pParent->GetWorldPosition() + m_LocalPosition;
		}
	}

	m_PositionIsDirty = false;
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}
#pragma endregion

#pragma region Parenting
void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	//Check if new parent is valid (not itself or one of its children)
	if (IsChildOf(pParent) || pParent == this || m_pParent == pParent) return;

	//Update position, rotation and scale
	if (pParent == nullptr)
	{
		SetPosition(GetTransform().GetPosition().x, GetTransform().GetPosition().y);
	}
	else
	{
		if (keepWorldPosition)
		{
			auto newPos = GetTransform().GetPosition() - pParent->GetTransform().GetPosition();
			GetTransform().SetPosition(newPos.x, newPos.y, 0.0f);
		}
	}

	//Remove itself from previous parent (if any)
	if (m_pParent) m_pParent->RemoveChild(this);

	//Set the given parent on itself
	m_pParent = pParent;

	//Add itself as a child to the given parent
	if (m_pParent) m_pParent->AddChild(this);
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(pChild);
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild));
}

bool dae::GameObject::IsChildOf(GameObject* pObject) const
{
	for (auto child : m_pChildren)
	{
		if (child == pObject || child->IsChildOf(pObject)) return true;
	}

	return false;
}
#pragma endregion