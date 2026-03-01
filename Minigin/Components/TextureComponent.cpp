#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent(GameObject* pGameObject)
	: GameComponent(pGameObject),
	m_texture(nullptr)
{
}

void dae::TextureComponent::Render() const
{
	if (m_texture != nullptr)
	{
		const auto& pos = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::TextureComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{ 
	m_texture = std::move(texture); 
}


