#pragma once

#include <memory>

namespace dae
{
    enum class ControllerButton
    {
        DpadUp = 0x0001,
        DpadDown = 0x0002,
        DpadLeft = 0x0004,
        DpadRight = 0x0008,
        Start = 0x0010,
        Back = 0x0020,
        LeftThumb = 0x0040,
        RightThumb = 0x0080,
        LeftShoulder = 0x0100,
        RightShoulder = 0x0200,
        ButtonA = 0x1000,
        ButtonB = 0x2000,
        ButtonX = 0x4000,
        ButtonY = 0x8000
    };

    class Gamepad final {
    public:
        explicit Gamepad(uint16_t controllerIdx);
        ~Gamepad();

        void Update();

        bool IsDown(ControllerButton button) const;
        bool IsUp(ControllerButton button) const;
        bool IsPressed(ControllerButton button) const;

        uint16_t GetIndex() const;

    private:

        class GamepadImpl;
        std::unique_ptr<GamepadImpl> m_pImpl;
    };
}