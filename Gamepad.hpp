/**
 * Gamepad.hpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#ifndef CPP18_ASTEROIDS_GAMEPAD_HPP
#define CPP18_ASTEROIDS_GAMEPAD_HPP

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGamepad/QGamepad>

namespace asteroids
{

/**
 * Class that represents a gamepad
 */
class Gamepad : public QObject
{
Q_OBJECT

public:

    /**
     * @brief Constructor
     */
    explicit Gamepad(QObject *parent = nullptr);

    /**
     * @brief tries to find a gamepad and maps all events to the corresponding methods
     * @return true if a gamepad was found otherwise false
     */
    bool init();

    /**
     * @brief returns whether or not the button A is pressed
     * @return true if A is pressed otherwise false
     */
    bool isAPressed() const;

    /**
     * @brief returns whether or not the button B is pressed
     * @return true if B is pressed otherwise false
     */
    bool isBPressed() const;

    /**
     * @brief returns the R2 axis value
     * @return R2 axis value
     */
    double getR2() const;

    /**
     * @brief returns the horizontal L axis value
     * @return horizontal L axis value
     */
    double getLeftX() const;

    /**
     * @brief returns the vertical L axis value
     * @return vertical L axis value
     */
    double getLeftY() const;

    /**
     * @brief returns whether or not the button R1 is pressed
     * @return true if R1 is pressed otherwise false
     */
    bool isR1Pressed() const;

    /**
     * @brief returns whether or not the button L1 is pressed
     * @return true if L1 is pressed otherwise false
     */
    bool isL1Pressed() const;

private:

    /// the underlying QGamepad object
    QGamepad *m_gamepad;

    /// flag whether or not the button A is pressed
    bool m_aPressed;

    /// flag whether or not the button B is pressed
    bool m_bPressed;

    /// R2 axis value
    double m_r2;

    /// horizontal L value
    double m_leftX;

    /// vertical L value
    double m_leftY;

    /// flag whether or not the button L1 is pressed
    bool m_l1Pressed;

    /// flag whether or not the button R1 is pressed
    bool m_r1Pressed;

};

}

#endif
