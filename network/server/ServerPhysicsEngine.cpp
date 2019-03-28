
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


std::list<std::pair<int, int>> ServerPhysicsEngine::detect_collisions()
{
    std::list<std::pair<int, int>> collisions;
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, PhysicalBullet::Ptr>::iterator b_it;
    map<int, Hittable::Ptr>::iterator h_it;


    map<int, PhysicalMissile::Ptr>::iterator m_it;
    m_it = m_missiles.begin();
    while (m_it != m_missiles.end())
    {
        PhysicalMissile::Ptr m = m_it->second;
        h_it = m_hittables.begin();
        while (h_it != m_hittables.end())
        {
            if (m->getShooterId() != h_it->second->getId() && h_it->second->hit(m->getPosition(), 50))
            {
                if (m_hittables.size() > 1)
                {
                    m->destroy();
                    int health = h_it->second->getHealth();
                    h_it->second->setHealth(health - min(3, health % 10));
                    collisions.push_back(std::pair<int, int>(m->getId(), h_it->second->getId()));
                }
            }
            if (h_it->second->getHealth() == 0 && m_hittables.size() > 1)
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
        if (!m->alive())
        {
            m_it = m_missiles.erase(m_it);
        }
        else
        {
            m_it++;
        }
    }


    //Move bullets and test for hits
    b_it = m_bullets.begin();
    while (b_it != m_bullets.end())
    {
        PhysicalBullet::Ptr b = b_it->second;
        // Check for collisions with present objects
        p_it = m_objects.begin();
        while (p_it != m_objects.end())
        {

            //std::cout << "bullet" << b->get_id() << "ast " << p_it->second->get_id() << std::endl;
            if (p_it->second->collision(b->getPosition(), b->radius()))
            {
                // Mark bulled as killed
                b->destroy();
                for (auto h : m_hittables)
                {
                    if (h.second->getId() == b->get_shooter_id() && h.second->getHealth() % 10 != 0)
                    {
                        h.second->setHealth(h.second->getHealth() + 1);
                    }
                }

                //std::cout << "collison " << b->get_id() << " " << p_it->second->get_id() << std::endl;

                collisions.push_back(std::pair<int, int>(b->get_id(), p_it->second->get_id()));

                p_it = m_objects.erase(p_it);
            }
            else
            {
                p_it++;
            }
        }


        h_it = m_hittables.begin();
        while (h_it != m_hittables.end())
        {
            if (b->get_shooter_id() != h_it->second->getId() && h_it->second->hitBullet(*b))
            {
                b->destroy();
                if (m_hittables.size() > 1)
                {
                    h_it->second->setHealth(h_it->second->getHealth() - 1);
                    collisions.push_back(std::pair<int, int>(b->get_id(), h_it->second->getId()));
                }
            }
            if (h_it->second->getHealth() == 0 && m_hittables.size() > 1)
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
            if (h_it->second->hitAsteroid(*p_it->second))
            {
                
                collisions.push_back(std::pair<int, int>(h_it->second->getId(), p_it->second->get_id()));
                p_it = m_objects.erase(p_it);

                h_it->second->setHealth(h_it->second->getHealth() - 1);
            }
            else
            {
                p_it++;
            }
        }
        if (h_it->second->getHealth() == 0)
        {
            h_it = m_hittables.erase(h_it);
        }
        else
        {
            h_it++;
        }
    }



    return collisions;

}

} /* namespace asteroids */
