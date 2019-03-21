
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


void PhysicsEngine::addDestroyable(PhysicalObject::Ptr& obj)
{
    m_objects.insert(std::pair<int, PhysicalObject::Ptr>(curr_dest_id++, obj));
}

void PhysicsEngine::addBullet(Bullet::Ptr& bullet)
{
    //m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));
    m_bullets.insert(std::pair<int, Bullet::Ptr >(curr_bull_id++, bullet));
}

void PhysicsEngine::process()
{
    //list<PhysicalObject::Ptr>::iterator p_it;
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, Bullet::Ptr>::iterator b_it;

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
        Bullet::Ptr b = (*b_it).second;
        b->run();

        // Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*p_it).second->collision(b->getPosition(), b->radius()))
            {
                // Mark bulled as killed
                b->destroy();

                // Delete destroyed object
                p_it = m_objects.erase(p_it);

                // Add explosion
                m_particles.addEffect(ParticleEffect::createExplosionSphere(b->getPosition()));
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

    m_particles.update();
}

void PhysicsEngine::render()
{
   
    // Render all objects
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, Bullet::Ptr>::iterator b_it;
 

     for(p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
        {
            PhysicalObject::Ptr p = (*p_it).second;
            p->render();
        }

    // Render active bullets and delete inactive ones
    b_it = m_bullets.begin();
    while(b_it != m_bullets.end())
    {
        Bullet::Ptr b = (*b_it).second;
        b->render();
        b_it++;
    }

    m_particles.render();
    //cout << m_bullets.size() << endl;

}

} /* namespace asteroids */
