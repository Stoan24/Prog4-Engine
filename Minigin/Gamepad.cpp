#include "Gamepad.h"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#else
#include <SDL3/SDL.h>
#include <vector>
#endif

namespace dae
{
    struct Gamepad::GamepadImpl
    {
        uint16_t m_ControllerIndex{};

#ifdef _WIN32

        XINPUT_STATE m_CurrentState{};
        XINPUT_STATE m_PreviousState{};
        uint16_t m_ButtonsPressedThisFrame{};
        uint16_t m_ButtonsReleasedThisFrame{};


        GamepadImpl(uint16_t controllerIndex) : m_ControllerIndex(controllerIndex)
        {
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        }

        void Update()
        {
            CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            XInputGetState(m_ControllerIndex, &m_CurrentState);

            auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
        }

        bool IsDown(ControllerButton button) const { return m_ButtonsPressedThisFrame & static_cast<uint16_t>(button); }
        bool IsUp(ControllerButton button) const { return m_ButtonsReleasedThisFrame & static_cast<uint16_t>(button); }
        bool IsPressed(ControllerButton button) const { return m_CurrentState.Gamepad.wButtons & static_cast<uint16_t>(button); }

#else

        SDL_Gamepad* m_pGamepad{ nullptr };
        uint16_t m_PreviousState{ 0 };
        uint16_t m_CurrentState{ 0 };
        uint16_t m_ButtonsPressedThisFrame{ 0 };
        uint16_t m_ButtonsReleasedThisFrame{ 0 };
        std::vector<ControllerButton> m_Buttons;

        GamepadImpl(uint16_t controllerIndex) : m_ControllerIndex(controllerIndex) {
            m_Buttons = {
                ControllerButton::DpadUp, ControllerButton::DpadDown, ControllerButton::DpadLeft, ControllerButton::DpadRight,
                ControllerButton::Start, ControllerButton::Back, ControllerButton::LeftThumb, ControllerButton::RightThumb,
                ControllerButton::LeftShoulder, ControllerButton::RightShoulder,
                ControllerButton::ButtonA, ControllerButton::ButtonB, ControllerButton::ButtonX, ControllerButton::ButtonY
            };
            Connect();
        }

        ~GamepadImpl()
        {
            if (m_pGamepad) SDL_CloseGamepad(m_pGamepad);
        }

        void Update()
        {
            if (!m_pGamepad)
            {
                Connect();
                if (!m_pGamepad) return;
            }

            m_PreviousState = m_CurrentState;
            m_CurrentState = 0;

            for (const auto& button : m_Buttons)
            {
                if (SDL_GetGamepadButton(m_pGamepad, ControllerToSDL(button)))
                {
                    m_CurrentState |= static_cast<uint16_t>(button);
                }
            }

            uint16_t buttonChanges = m_CurrentState ^ m_PreviousState;
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState);
        }

        bool IsDown(ControllerButton button) const
        {
            return m_ButtonsPressedThisFrame & static_cast<uint16_t>(button);
        }

        bool IsUp(ControllerButton button) const
        {
            return m_ButtonsReleasedThisFrame & static_cast<uint16_t>(button);
        }

        bool IsPressed(ControllerButton button) const
        {
            return m_CurrentState & static_cast<uint16_t>(button);
        }

        void Connect()
        {
            SDL_InitSubSystem(SDL_INIT_GAMEPAD);

            int count{};
            SDL_JoystickID* joysticks = SDL_GetGamepads(&count);

            if (joysticks && m_ControllerIndex < (uint16_t)count)
            {
                m_pGamepad = SDL_OpenGamepad(joysticks[m_ControllerIndex]);
            }
            
            SDL_free(joysticks);
        }

        SDL_GamepadButton ControllerToSDL(ControllerButton button) const {
            switch (button) {
            case ControllerButton::DpadUp: return SDL_GAMEPAD_BUTTON_DPAD_UP;
            case ControllerButton::DpadDown: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
            case ControllerButton::DpadLeft: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
            case ControllerButton::DpadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
            case ControllerButton::Start: return SDL_GAMEPAD_BUTTON_START;
            case ControllerButton::Back: return SDL_GAMEPAD_BUTTON_BACK;
            case ControllerButton::LeftThumb: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
            case ControllerButton::RightThumb: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
            case ControllerButton::LeftShoulder: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
            case ControllerButton::RightShoulder: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
            case ControllerButton::ButtonA: return SDL_GAMEPAD_BUTTON_SOUTH;
            case ControllerButton::ButtonB: return SDL_GAMEPAD_BUTTON_EAST;
            case ControllerButton::ButtonX: return SDL_GAMEPAD_BUTTON_WEST;
            case ControllerButton::ButtonY: return SDL_GAMEPAD_BUTTON_NORTH;
            default: return SDL_GAMEPAD_BUTTON_INVALID;
            }
        }
#endif
    };

    Gamepad::Gamepad(uint16_t controllerIdx)
        : m_pImpl(std::make_unique<GamepadImpl>(controllerIdx))
    {
    }

    Gamepad::~Gamepad() = default;

    void Gamepad::Update() { m_pImpl->Update(); }

    bool Gamepad::IsPressed(ControllerButton button) const
    {
        return m_pImpl->IsPressed(button);
    }

    bool Gamepad::IsDown(ControllerButton button) const
    {
        return m_pImpl->IsDown(button);
    }

    bool Gamepad::IsUp(ControllerButton button) const
    {
        return m_pImpl->IsUp(button);
    }

    uint16_t Gamepad::GetIndex() const { return m_pImpl->m_ControllerIndex; }
}