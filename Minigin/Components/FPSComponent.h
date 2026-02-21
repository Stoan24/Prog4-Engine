#pragma once
#include "GameComponent.h"

namespace dae
{
	class TextComponent;

	class FPSComponent final : public GameComponent
	{
	public:
		explicit FPSComponent(GameObject* gameObject);
		virtual ~FPSComponent() = default;

		void Update() override;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;



	private:
		TextComponent* m_pTextComponent{ nullptr };
		float m_updateTimer{ 0.f };
		int m_frameCount{ 0 };
		float m_frameTime{ 0.5f };
	};
}