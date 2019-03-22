#ifndef CPP18_ASTEROIDS_CONTROLLER_HPP
#define CPP18_ASTEROIDS_CONTROLLER_HPP

#include <QtCore/Qt>
#include <map>
#include "Gamepad.hpp"
#include "physics/Hittable.hpp"
#include "physics/PhysicsEngine.hpp"

namespace asteroids
{

class Controller
{

public:

    Controller();

    void keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player,
                    PhysicsEngine::Ptr& physicsEngine, int elapsed_time);

    void gamepadControl(Hittable::Ptr& player, PhysicsEngine::Ptr& physicsEngine, int elapsed_time);

private:

    static const vector<Qt::Key> mapToQt;

    static const vector<Transformable::RotationTransform> mapToAngle;

    std::vector<int> m_keys;

    int m_cooldownPlayer;

    int m_cooldownEnemy;

    bool m_gamepadAvailable;

    Gamepad m_gamepad;

};

}

#endif
