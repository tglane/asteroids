#ifndef CPP18_ASTEROIDS_CONTROLLER_HPP
#define CPP18_ASTEROIDS_CONTROLLER_HPP

#include <QtCore/Qt>
#include <map>
#include "physics/Hittable.hpp"
#include "physics/PhysicsEngine.hpp"

namespace asteroids
{

class Controller
{

public:

    Controller();

    void keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player, Hittable::Ptr& enemy,
                    PhysicsEngine::Ptr& physicsEngine, int elapsed_time);

private:

    int m_cooldownPlayer;

    int m_cooldownEnemy;

};

}

#endif
