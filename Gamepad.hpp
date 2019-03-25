#ifndef CPP18_ASTEROIDS_GAMEPAD_HPP
#define CPP18_ASTEROIDS_GAMEPAD_HPP

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGamepad/QGamepad>

namespace asteroids
{

class Gamepad : public QObject
{
Q_OBJECT

public:

    explicit Gamepad(QObject *parent = nullptr);

    bool init();

    bool isAPressed() const;

    double getR2() const;

    double getLeftX() const;

    double getLeftY() const;

    bool isR1Pressed() const;

    bool isL1Pressed() const;

private:

    QGamepad *m_gamepad;

    bool m_aPressed;

    double m_r2;

    double m_leftX;

    double m_leftY;

    bool m_l1Pressed;

    bool m_r1Pressed;

};

}

#endif
