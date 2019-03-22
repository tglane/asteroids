
/*
 *  ServerPhysicsEngine.cpp
 *
 *  @date 19.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#include "ServerPhysicsEngine.hpp"

#include <iostream>
using namespace std;

namespace asteroids
{

void ServerPhysicsEngine::addDestroyable(PhysicalObject::Ptr& obj)
{
    m_objects.push_back(obj);
}

void ServerPhysicsEngine::addHittable(Hittable::Ptr& h)
{
    m_hittables.push_back(h);
}

void ServerPhysicsEngine::addBullet(PhysicalBullet::Ptr& bullet)
{
    //m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));
    m_bullets.push_back(bullet);
}

std::list<std::pair<int, int>> ServerPhysicsEngine::process()
{
    std::list<std::pair<int, int>> collisions;
    //list<PhysicalObject::Ptr>::iterator p_it;
    list<PhysicalObject::Ptr>::iterator p_it;
    list<PhysicalBullet::Ptr>::iterator b_it;
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
        PhysicalBullet::Ptr b = *b_it;
        b->run();

// Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if ((*p_it)->collision(b->getPosition(), b->radius()))
            {
                collisions.push_back(std::pair<int, int>(b->get_id(), (*p_it)->get_id()));
                // Mark bulled as killed
                b->destroy();

                // Delete destroyed object
                p_it = m_objects.erase(p_it);

            }
            p_it++;
        }

        h_it = m_hittables.begin();
        while (h_it != m_hittables.end())
        {
            //std::cout << "hittable " << (*h_it)->getPosition()[0] << " " << (*h_it)->getPosition()[1]<< " "<< (*h_it)->getPosition()[2] << std::endl;
            //std::cout << "hittable " << (*h_it)->getId() << " " << b->get_shooter_id() << " " << (*h_it)->hit(*b) << std::endl;


            if (b->get_shooter_id() != (*h_it)->getId() && (*h_it)->hit(*b))
            {
                std::cout << "Treffer an Spieler " << (*h_it)->getId() << std::endl;
                collisions.push_back(std::pair<int, int>(b->get_id(), (*h_it)->getId()));
            }
            h_it++;
        }

        // Check if bullet is dead. If it is, remove from
        // bullet list. Otherwise continue with next bullet.
        if (!b->alive())
        {
            std::cout << "ADFSAD" << std::endl;
            b_it = m_bullets.erase(b_it);
        }
        else
        {
            b_it++;
        }
    }

    return collisions;

}

} /* namespace asteroids */
