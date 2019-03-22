
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
    m_objects.insert(std::pair<int, PhysicalObject::Ptr>(obj->get_id(), obj));
}

void ServerPhysicsEngine::addHittable(Hittable::Ptr& h)
{
    m_hittables.insert(std::pair<int, Hittable::Ptr>(h->getId(), h));
}

void ServerPhysicsEngine::addBullet(PhysicalBullet::Ptr& bullet)
{

    m_bullets.insert(std::pair<int, PhysicalBullet::Ptr >(bullet->get_id(), bullet));
}


bool ServerPhysicsEngine::gameOver()
{
    return m_hittables.size() <= 1;
}


std::list<std::pair<int, int>> ServerPhysicsEngine::process(int time_elapsed)
{
    std::list<std::pair<int, int>> collisions;
    //list<PhysicalObject::Ptr>::iterator p_it;
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, PhysicalBullet::Ptr>::iterator b_it;
    map<int, Hittable::Ptr>::iterator h_it;

    // Move all objects
    for (p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
    {
        PhysicalObject::Ptr p = p_it->second;
        p->move();
    }

    //Move bullets and test for hits
    b_it = m_bullets.begin();
    while (b_it != m_bullets.end())
    {
        PhysicalBullet::Ptr b = b_it->second;
        b->run(time_elapsed);

// Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {
            if (p_it->second->collision(b->getPosition(), b->radius()))
            {
                collisions.push_back(std::pair<int, int>(b->get_id(), p_it->second->get_id()));
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


            if (b->get_shooter_id() != h_it->second->getId() && h_it->second->hit(*b))
            {
                std::cout << "Treffer an Spieler " << (*h_it).second->getId() << std::endl;
                b->destroy();
                if (m_hittables.size() > 1)
                {
                    h_it->second->setHealth(h_it->second->getHealth() - 1);
                    if (h_it->second->getHealth() == 0)
                    {
                        //m_particles.addEffect(ParticleEffect::createExplosionSphere((*h_it)->getPosition()));
                        h_it = m_hittables.erase(h_it);
                    }
                }
            }
            if (h_it != m_hittables.end()) {
                h_it++;
            }
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
