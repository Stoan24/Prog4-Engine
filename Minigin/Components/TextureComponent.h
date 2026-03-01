#pragma once
#include <string>
#include <memory>
#include "GameComponent.h"
#include <GameObject.h>
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextureComponent final : public GameComponent
	{
	public:
		explicit TextureComponent(GameObject* pGameObject);
		virtual ~TextureComponent() = default;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;


		void Render() const override;

		void SetTexture(const std::string& fileName);
		void SetTexture(std::shared_ptr<Texture2D> texture);

	private:
		std::shared_ptr<Texture2D> m_texture{};
	};
}