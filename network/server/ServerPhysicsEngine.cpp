
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

    //Move bullets and test for hits
    b_it = m_bullets.begin();
    while (b_it != m_bullets.end())
    {
        PhysicalBullet::Ptr b = b_it->second;

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
            else
            {
                p_it++;
            }
        }

        h_it = m_hittables.begin();
        while (h_it != m_hittables.end())
        {

            if (b->get_shooter_id() != h_it->second->getId() && h_it->second->hit(*b))
            {
                std::cout << "Treffer an Spieler " << (*h_it).second->getId() << std::endl;
                b->destroy();
                if (m_hittables.size() > 1)
                {
                    collisions.push_back(std::pair<int, int>(b->get_id(), h_it->second->getId()));
                    h_it->second->setHealth(h_it->second->getHealth() - 1);
                    if (h_it->second->getHealth() == 0)
                    {
                        std::cout << "Player destroyed: " << h_it->second->getId() << std::endl;
                        h_it = m_hittables.erase(h_it);
                    }
                }
            }
            if (h_it != m_hittables.end()) {
                h_it++;
            }
        }
    }

    return collisions;

}

} /* namespace asteroids */
