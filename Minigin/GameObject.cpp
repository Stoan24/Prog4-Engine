#include "Transform.h"
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject()
{
	AddComponent<Transform>();
}

dae::GameObject::~GameObject()
{
	//m_pChildren.clear();
}

void dae::GameObject::Update()
{
	for (const auto& component : m_pComponents)
	{
		component->Update();
	}

	for (const auto& child : m_pChildren)
	{
		child->Update();
	}

	DestroyMarkedComponents();
}

void dae::GameObject::Render() const
{
	for(const auto& component : m_pComponents)
	{
		component->Render();
	}

	for (const auto& child : m_pChildren)
	{
		child->Render();
	}
}

void dae::GameObject::RenderGUI()
{
	for (const auto& component : m_pComponents)
	{
		component->RenderGUI();
	}
}

void dae::GameObject::MarkForDestruction()
{
	m_IsMarkForDestruction = true;
	
	for(const auto& child : m_pChildren)
	{
		child->MarkForDestruction();
	}
}

void dae::GameObject::MarkToDestroy()
{
	m_IsMarkForDestruction = true;

	for (auto& child : m_pChildren)
	{
		child->MarkToDestroy();
	}
}

#pragma region Parenting
void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	//Check if new parent is valid (not itself or one of its children)
	if (IsChildOf(pParent) || pParent == this || m_pParent == pParent) return;

	auto transform = GetComponent<Transform>();
	//Update position, rotation and scale
	if (pParent == nullptr)
	{
		transform->SetLocalPosition(transform->GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			transform->SetLocalPosition(transform->GetWorldPosition() - pParent->GetComponent<Transform>()->GetWorldPosition());
		}
		else
		{
			transform->SetPositionDirty();
		}
	}

	//Remove itself from previous parent (if any)
	if (m_pParent) m_pParent->RemoveChild(this);

	//Set the given parent on itself
	m_pParent = pParent;

	//Add itself as a child to the given parent
	if (m_pParent) m_pParent->AddChild(this);
}

std::vector<dae::GameObject*> dae::GameObject::GetChildren() const
{
	std::vector<GameObject*> pointers;
	pointers.reserve(m_pChildren.size());

	for (const auto& child : m_pChildren)
	{
		pointers.push_back(child);
	}
	return pointers;
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.emplace_back(std::move(pChild));
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild));
}

bool dae::GameObject::IsChildOf(GameObject* pObject) const
{
	for (const auto& child : m_pChildren)
	{
		if (!child) continue;

		if (child == pObject || child->IsChildOf(pObject)) return true;
	}

	return false;
}

void dae::GameObject::DestroyMarkedComponents()
{
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
		[](const std::unique_ptr<GameComponent>& component) {
			return component->IsMarkedForDestruction();
		}),
		m_pComponents.end());
}
#pragma endregion