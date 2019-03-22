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

    bool init();

    bool isAPressed() const;

    double getR2() const;

    double getLeftX() const;

    double getLeftY() const;

private:

    QGamepad *m_gamepad;

    bool m_aPressed;

    double m_r2;

    double m_leftX;

    double m_leftY;

};

}

#endif
