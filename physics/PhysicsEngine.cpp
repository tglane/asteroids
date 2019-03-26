
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


bool PhysicsEngine::process(int elapsed_time)
{
    bool game_over = BasePhysicsEngine::process(elapsed_time);

    m_particles.update();

    return game_over;
}

void PhysicsEngine::render()
{
    // Render all objects
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, PhysicalBullet::Ptr>::iterator b_it;
 

     for(p_it = m_objects.begin(); p_it != m_objects.end(); p_it++)
        {
            PhysicalObject::Ptr p = (*p_it).second;
            p->render();
        }

    // Render active bullets and delete inactive ones
    b_it = m_bullets.begin();
    while(b_it != m_bullets.end())
    {
        PhysicalBullet::Ptr b = (*b_it).second;
        b->render();
        b_it++;
    }

    m_particles.render();
}

void PhysicsEngine::process_collisions(int id_one, int id_two)
{
    check_id_type(id_one);
    check_id_type(id_two);
    m_particles.update();
}

void PhysicsEngine::check_id_type(int id_to_check)
{
    if((id_to_check >> 24) == 0)
    {
        if(m_objects.count(id_to_check == 1))
        {
            m_particles.addEffect(ParticleEffect::createExplosionSphere(m_objects[id_to_check]->getPosition()));
            m_objects.erase(id_to_check);
        }
    }
    else
    {
        if((id_to_check & 0xFFFFFF) != 0)
        {

            if(m_bullets.count(id_to_check) == 1) {

                m_bullets[id_to_check]->destroy();
                m_bullets.erase(id_to_check);
            }
        }
        else
        {
            std::cout << id_to_check << std::endl;
            //TODO change health of spaceship if collision with spaceship
            if(m_hittables.count(id_to_check) == 1) {
                int health = m_hittables[id_to_check]->getHealth();
                m_hittables[id_to_check]->setHealth(health - 1);
                std::cout << health << std::endl;
                if (health <= 1)
                {
                    m_particles.addEffect(ParticleEffect::createExplosionSphere(m_hittables[id_to_check]->getPosition()));
                    m_hittables.erase(id_to_check);
                }
            }
        }
    }
}

} /* namespace asteroids */
