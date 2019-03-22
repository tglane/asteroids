#include "Gamepad.hpp"

namespace asteroids {

bool Gamepad::init()
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty())
    {
        return false;
    }
    m_gamepad = new QGamepad(*gamepads.begin(), this);
    connect(m_gamepad, &QGamepad::buttonAChanged, this, [this](bool pressed)
    {
        m_aPressed = pressed;
    });
    connect(m_gamepad, &QGamepad::buttonR2Changed, this, [this](double value)
    {
        m_r2 = value;
    });
    connect(m_gamepad, &QGamepad::axisLeftXChanged, this, [this](double value)
    {
        m_leftX = value;
    });
    connect(m_gamepad, &QGamepad::axisLeftYChanged, this, [this](double value)
    {
        m_leftY = value;
    });
    return true;
}

bool Gamepad::isAPressed() const
{
    return m_aPressed;
}

double Gamepad::getR2() const
{
    return m_r2;
}

double Gamepad::getLeftX() const
{
    return m_leftX;
}

double Gamepad::getLeftY() const
{
    return m_leftY;
}

}
