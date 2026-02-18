#include <sstream>
#include <iomanip>

#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"


dae::FPSComponent::FPSComponent(GameObject* gameObject)
	:GameComponent(gameObject)
{
	m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
}


void dae::FPSComponent::Update()
{
	const float deltaTime = GameTime::GetInstance().GetDeltaTime();

	m_updateTimer += deltaTime;
	m_frameCount++;

	if (m_updateTimer >= m_frameTime)
	{
		const float fps = static_cast<float>(m_frameCount) / m_updateTimer;

		if (m_pTextComponent)
		{
			std::stringstream ss;

			ss << std::fixed << std::setprecision(1) << fps << " FPS";

			m_pTextComponent->SetText(ss.str());
		}

		m_frameCount = 0;
		m_updateTimer -= m_frameTime;
	}
}