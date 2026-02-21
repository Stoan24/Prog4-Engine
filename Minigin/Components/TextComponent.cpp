#include "TextureComponent.h"
#include "TextComponent.h"
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* gameObject, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
	: GameComponent(gameObject),
	m_needsUpdate(true),
	m_text(text),
	m_color(color),
	m_font(std::move(font)),
	m_textTexture(nullptr)
{
	m_pTextureComponent = GetGameObject()->GetComponent<TextureComponent>();
	if (!m_pTextureComponent)
	{
		throw std::runtime_error("Textcomponent required a TextureComponent to work");
	}
}

void dae::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		SDL_DestroySurface(surf);

		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		auto newTexture = std::make_shared<Texture2D>(texture);
		m_pTextureComponent->SetTexture(newTexture);
		m_textTexture.reset();
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		m_pTextureComponent->Render();
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y)
{
	GetGameObject()->GetTransform().SetPosition(x, y);
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_color = color;
	m_needsUpdate = true;
}


