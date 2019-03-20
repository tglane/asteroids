
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


#include "PhysicsEngine.hpp"

#include <iostream>
using namespace std;

namespace asteroids
{


void BasePhysicsEngine::addDestroyable(PhysicalObject::Ptr& obj)
{
    m_objects.push_back(obj);
}

void BasePhysicsEngine::addBullet(Bullet::Ptr& bullet)
{
    m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));
    m_bullets.push_back(bullet);
}

void BasePhysicsEngine::process()
{
    //list<PhysicalObject::Ptr>::iterator p_it;
    list<PhysicalObject::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;

    // Move all objects
    for (p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
    {
        PhysicalObject::Ptr p = *p_it;
        p->move();
    }

    //Move bullets and test for hits
    b_it = m_bullets.begin();
    while (b_it != m_bullets.end())
    {
        Bullet::Ptr b = *b_it;
        b->run();

        // Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*p_it)->collision(b->getPosition(), b->radius()))
            {
                // Mark bulled as killed
                b->destroy();
                handle_collision(b, *p_it)

                // Delete destroyed object
                p_it = m_objects.erase(p_it);
            }
            p_it++;
        }

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

}

} /* namespace asteroids */
