#include "Gamepad.h"
#include <minwinbase.h>

dae::Gamepad::Gamepad(unsigned int controllerIdx) : m_ControllerIndex(controllerIdx)
{
    ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
}

void dae::Gamepad::Update()
{
    m_PreviousState = m_CurrentState;
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    XInputGetState(m_ControllerIndex, &m_CurrentState);


    auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
    m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
    m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Gamepad::IsDown(unsigned int button) const 
{
    return m_ButtonsPressedThisFrame & button;
}

bool dae::Gamepad::IsUp(unsigned int button) const 
{
    return m_ButtonsReleasedThisFrame & button;
}

bool dae::Gamepad::IsPressed(unsigned int button) const 
{
    return m_CurrentState.Gamepad.wButtons & button;
}
