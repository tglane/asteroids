#include "Gamepad.hpp"

namespace asteroids {

Gamepad::Gamepad(QObject *parent) : QObject(parent),
                                    m_aPressed(false), m_r2(0), m_leftX(0), m_leftY(0), m_r1Pressed(false), m_l1Pressed(false) {}

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
    connect(m_gamepad, &QGamepad::buttonR1Changed, this, [this](bool pressed)
    {
        m_r1Pressed = pressed;
    });
    connect(m_gamepad, &QGamepad::buttonL1Changed, this, [this](bool pressed)
    {
        m_l1Pressed = pressed;
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

bool Gamepad::isR1Pressed() const
{
    return m_r1Pressed;
}

bool Gamepad::isL1Pressed() const
{
    return m_l1Pressed;
}

}
