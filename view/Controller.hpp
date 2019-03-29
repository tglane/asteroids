/**
 * Controller.hpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#ifndef CPP18_ASTEROIDS_CONTROLLER_HPP
#define CPP18_ASTEROIDS_CONTROLLER_HPP

#include "util/gl_includes.h"
#include <QtCore/Qt>
#include <QtMultimedia/QSoundEffect>
#include "Gamepad.hpp"
#include "physics/Hittable.hpp"
#include "physics/PhysicsEngine.hpp"
#include "network/client/udpclient.hpp"

namespace asteroids
{

/**
 * Class that handles all inputs from the player while fighting
 */
class Controller
{

public:

    /**
     * @brief Constructor
     */
    Controller();

    /**
     * @brief searches for a gamepad
     * @return true if a gamepad is available
     */
    bool gamepadAvailable();

    /**
     * @brief control by keyboard
     * @param keyStates map from all keys to their status
     * @param player the player that gets controlled
     * @param physicsEngine the physics engine that is used in the game to handle bullets and missiles
     * @param client the udp client that is used in the game
     * @param elapsed_time the time that has passed since the last step used to prevent depending on the framerate of the game
     */
    void keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player,
                    PhysicsEngine::Ptr& physicsEngine, udpclient& client, int elapsed_time);

    /**
     * @brief control by gamepad
     * @param player the player that gets controlled
     * @param physicsEngine the physics engine that is used in the game to handle bullets and missiles
     * @param client the udp client that is used in the game
     * @param elapsed_time the time that has passed since the last step used to prevent depending on the framerate of the game
     */
    void gamepadControl(Hittable::Ptr& player, PhysicsEngine::Ptr& physicsEngine, udpclient& client, int elapsed_time);

private:

    /// map from qt keys to integers realized by a vector
    static const vector<Qt::Key> mapToQt;
    /// map from rotations to integers realized by a vector
    static const vector<Transformable::RotationTransform> mapToRotation;
    /// number of frames that it takes to reach maximum speed with keyboard control
    static const int framesToMaxSpeed;
    /// minimum speed
    static const float minSpeed;
    /// maximum speed
    static const float maxSpeed;
    /// number of frames that it takes to reach maximum rotation speed with keyboard control
    static const int framesToMaxRot;
    /// maximum rotation speed
    static const float maxRot;

    /// counter that counts how long a key has been pressed for all relevant keys
    std::vector<int> m_keys;

    /// counter to realize cooldown of the bullets
    int m_bulletCooldown;

    /// counter to realize cooldown of the missiles
    int m_missileCooldown;

    /// flag whether or not a gamepad is available
    bool m_gamepadAvailable;

    /// gamepad
    Gamepad m_gamepad;

    /// counter that counts how long R1 on the gamepad has been pressed
    int m_gamepadR1;

    /// counter that counts how long L1 on the gamepad has been pressed
    int m_gamepadL1;

    /// sound of shooting
    QSoundEffect m_bulletSound;

};

}

#endif
