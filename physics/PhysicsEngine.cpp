
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
    m_objects.push_back(obj);
}

void PhysicsEngine::addHittable(Hittable::Ptr& h)
{
    m_hittables.push_back(h);
}

void PhysicsEngine::addBullet(Bullet::Ptr& bullet)
{
    //m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));
    m_bullets.push_back(bullet);
}

bool PhysicsEngine::process(int elapsed_time)
{
    bool gameOver = false;
    if (m_hittables.size() == 1)
    {
        gameOver = true;
    }
    //list<PhysicalObject::Ptr>::iterator p_it;
    list<PhysicalObject::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;
    list<Hittable::Ptr>::iterator h_it;

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
        b->run(elapsed_time);

        // Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*p_it)->collision(b->getPosition(), b->radius()))
            {
                // Mark bulled as killed
                b->destroy();
                for (Hittable::Ptr& h : m_hittables)
                {
                    if (h->getId() == b->get_shooter_id() && h->getHealth() % 10 != 0)
                    {
                        h->setHealth(h->getHealth() + 1);
                    }
                }

                // Delete destroyed object
                p_it = m_objects.erase(p_it);

                // Add explosion
                m_particles.addEffect(ParticleEffect::createExplosionSphere(b->getPosition()));
            }
            p_it++;
        }

        h_it = m_hittables.begin();
        while (h_it != m_hittables.end())
        {
            if (b->get_shooter_id() != (*h_it)->getId() && (*h_it)->hitBullet(*b))
            {
                b->destroy();
                if (m_hittables.size() > 1)
                {
                    (*h_it)->setHealth((*h_it)->getHealth() - 1);
                    if ((*h_it)->getHealth() % 10 == 0)
                    {
                        m_particles.addEffect(ParticleEffect::createExplosionSphere((*h_it)->getPosition()));
                    }
                }
            }
            if ((*h_it)->getHealth() == 0 && m_hittables.size() > 1)
            {
                h_it = m_hittables.erase(h_it);
            }
            else
            {
                h_it++;
            }
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

    // Check for asteroid collision
    h_it = m_hittables.begin();
    while (h_it != m_hittables.end())
    {
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*h_it)->hitAsteroid(**p_it))
            {
                // Add explosion
                m_particles.addEffect(ParticleEffect::createExplosionSphere((*p_it)->getPosition()));

                // Delete destroyed object
                p_it = m_objects.erase(p_it);

                (*h_it)->setHealth((*h_it)->getHealth() - 1);
                if ((*h_it)->getHealth() % 10 == 0)
                {
                    m_particles.addEffect(ParticleEffect::createExplosionSphere((*h_it)->getPosition()));
                    break;
                }
            }
            else
            {
                p_it++;
            }
        }
        if ((*h_it)->getHealth() == 0)
        {
            h_it = m_hittables.erase(h_it);
        }
        else
        {
            h_it++;
        }
    }

    m_particles.update();

    return gameOver;
}

void PhysicsEngine::render()
{
    // Render all objects
    list<PhysicalObject::Ptr>::iterator p_it;
    list<Bullet::Ptr>::iterator b_it;
 

     for(p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
        {
            PhysicalObject::Ptr p = *p_it;
            p->render();
        }

    // Render active bullets and delete inactive ones
    b_it = m_bullets.begin();
    while(b_it != m_bullets.end())
    {
        Bullet::Ptr b = (*b_it);
        b->render();
        b_it++;
    }

    m_particles.render();
}

} /* namespace asteroids */
