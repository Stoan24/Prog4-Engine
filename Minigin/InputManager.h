#pragma once
#include "Singleton.h"
#include <memory>
#include "Commands.h"
#include <vector>
#include "Gamepad.h"

namespace dae
{
	enum class KeyState
	{
		Down,
		Up,
		Pressed
	};


	struct KeyboardBinding 
	{
		SDL_Scancode key;
		KeyState state;
		std::unique_ptr<Command> command;
	};

	struct ControllerBinding 
	{
		ControllerButton buttonBitmask;
		KeyState state;
		unsigned int controllerIndex;
		std::unique_ptr<Command> command;
	};


	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		bool ProcessInput();

		//Keyboard Keys
		void BindKey(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command);
		void UnbindKey(SDL_Scancode key, KeyState state);

		//Controller Buttons
		void BindButton(unsigned int controllerIdx, ControllerButton button, KeyState state, std::unique_ptr<Command> command);
		void UnbindButton(unsigned int controllerIdx, ControllerButton button, KeyState state);

	private:
		int m_AmountOfControllers{ 2 };
		std::vector<std::unique_ptr<Gamepad>> m_pControllers;

		std::vector<KeyboardBinding> m_KeyboardBindings;
		std::vector<ControllerBinding> m_ControllerBindings;

	};

}
