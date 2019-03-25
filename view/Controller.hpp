#ifndef CPP18_ASTEROIDS_CONTROLLER_HPP
#define CPP18_ASTEROIDS_CONTROLLER_HPP

#include <QtCore/Qt>
#include "Gamepad.hpp"
#include "physics/Hittable.hpp"
#include "physics/PhysicsEngine.hpp"

namespace asteroids
{

class Controller
{

public:

    Controller();

    bool gamepadAvailable();

    void keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player,
                    PhysicsEngine::Ptr& physicsEngine, int elapsed_time);

    void gamepadControl(Hittable::Ptr& player, PhysicsEngine::Ptr& physicsEngine, int elapsed_time);

private:

    static const vector<Qt::Key> mapToQt;
    static const vector<Transformable::RotationTransform> mapToAngle;
    static const int framesToMaxSpeed;
    static const float minSpeed;
    static const float maxSpeed;
    static const int framesToMaxRot;
    static const float maxRot;

    std::vector<int> m_keys;

    int m_cooldownPlayer;

    bool m_gamepadAvailable;

    Gamepad m_gamepad;

    int m_gamepadR1;

    int m_gamepadL1;

};

}

#endif
