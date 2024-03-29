
/*
 *  PhysicsEngine.cpp
 *
 *  @date 19.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "BasePhysicsEngine.hpp"

#include <iostream>
using namespace std;

namespace asteroids
{

void BasePhysicsEngine::addDestroyable(PhysicalObject::Ptr obj)
{
    m_objects[obj->get_id()] =  obj;
}

void BasePhysicsEngine::addHittable(Hittable::Ptr h)
{
    m_hittables[h->getId()] = h;
}

void BasePhysicsEngine::addBullet(PhysicalBullet::Ptr bullet)
{
    m_bullets[bullet->get_id()] = bullet;
}

void BasePhysicsEngine::addMissile(PhysicalMissile::Ptr missile)
{
    m_missiles.insert(std::pair<int, PhysicalMissile::Ptr>(missile->getId(), missile));
}

bool BasePhysicsEngine::process(int elapsed_time)
{
    map<int, Hittable::Ptr>::iterator h_it;
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, PhysicalBullet::Ptr>::iterator b_it;
    map<int, Missile::Ptr>::iterator m_it;

    bool gameOver = m_hittables.size() == 1;

    // Move all objects
    for (p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
    {
        PhysicalObject::Ptr p = (*p_it).second;
        p->move();
    }

    //Move bullets and test for hits
    b_it = m_bullets.begin();

    while (b_it != m_bullets.end())
    {
        PhysicalBullet::Ptr b = (*b_it).second;
        b->run(elapsed_time);

        // Check if bullet is dead. If it is, remove from
        // bullet list. Otherwise continue with next bullet.
        if (!b->alive())
        {

            b_it = m_bullets.erase(b_it);
        }
        else
        {
            b_it++;
        }
    }

    m_it = m_missiles.begin();
    while (m_it != m_missiles.end())
    {
        Missile::Ptr m = (*m_it).second;
        m->run(elapsed_time);

        // Check if missile is dead. If it is, remove from
        // missile list. Otherwise continue with next missile.
        if (!m->alive())
        {
            m_it = m_missiles.erase(m_it);
        }
        else
        {
            m_it++;
        }
    }

    return gameOver;
}

} /* namespace asteroids */
