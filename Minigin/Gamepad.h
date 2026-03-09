#pragma once
#include "Xinput.h"

namespace dae
{
    class Gamepad final {
    public:
        explicit Gamepad(unsigned int controllerIdx);
        ~Gamepad() = default;

        void Update();

        bool IsDown(unsigned int button) const;
        bool IsUp(unsigned int button) const;
        bool IsPressed(unsigned int button) const;

        unsigned int GetIndex() const { return m_ControllerIndex; }

    private:
        unsigned int m_ControllerIndex;
        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};

        WORD m_ButtonsPressedThisFrame{};
        WORD m_ButtonsReleasedThisFrame{};
    };
}