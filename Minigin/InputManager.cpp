#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include <minwinbase.h>

dae::InputManager::InputManager() 
{
	for (int i = 0; i < m_AmountOfControllers; ++i)
	{
		m_Controllers.push_back(std::make_unique<Gamepad>(i));
	}
}

bool dae::InputManager::ProcessInput()
{
	for (auto& controller : m_pControllers) 
	{
		controller->Update();
	}

#pragma region Keyboard

	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_EVENT_QUIT) 
		{
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) 
		{
			for (auto& binding : m_KeyboardBindings) 
			{
				if (binding.key == e.key.scancode && binding.state == KeyState::Down) binding.command->Execute();
			}
		}
		if (e.type == SDL_EVENT_KEY_UP)
		{
			for (auto& binding : m_KeyboardBindings)
			{
				if (binding.key == e.key.scancode && binding.state == KeyState::Up) binding.command->Execute();
			}
		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) 
		{
			
		}

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	const bool* kbState = SDL_GetKeyboardState(NULL);
	for (auto& b : m_KeyboardBindings)
	{
		if (b.state == KeyState::Pressed && kbState[b.key]) b.command->Execute();
	}

#pragma endregion
	
	for (auto& b : m_ControllerBindings) 
	{
		const auto& controller = m_Controllers[b.controllerIndex];

		bool trigger = false;
		if (b.state == KeyState::Down && controller->IsDown(b.buttonBitmask)) trigger = true;
		if (b.state == KeyState::Up && controller->IsUp(b.buttonBitmask)) trigger = true;
		if (b.state == KeyState::Pressed && controller->IsPressed(b.buttonBitmask)) trigger = true;

		if (trigger) b.command->Execute();
	}

	return true;
}

void dae::InputManager::BindKey(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command) 
{
	m_KeyboardBindings.push_back({ key, state, std::move(command) });
}

void dae::InputManager::UnbindKey(SDL_Scancode key, KeyState state) 
{
	std::erase_if(m_KeyboardBindings, [=](const auto& b) 
		{
			return b.key == key && b.state == state;
		});
}

void dae::InputManager::BindButton(unsigned int controllerIdx, unsigned int bitmask, KeyState state, std::unique_ptr<Command> command) 
{
	m_ControllerBindings.push_back({ bitmask, state, controllerIdx, std::move(command) });
}

void dae::InputManager::UnbindButton(unsigned int controllerIdx, unsigned int bitmask, KeyState state) 
{
	std::erase_if(m_ControllerBindings, [=](const auto& b) 
		{
			return b.controllerIndex == controllerIdx && b.buttonBitmask == bitmask && b.state == state;
		});
}
