
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
    m_objects.insert(std::pair<int, PhysicalObject::Ptr>(obj->get_id(), obj));
}

void PhysicsEngine::addHittable(Hittable::Ptr& h)
{
    m_hittables.insert(std::pair<int, Hittable::Ptr>(h->getId(), h));
}

void PhysicsEngine::addBullet(Bullet::Ptr& bullet)
{

    //cout << "creating bullet " << bullet->get_id() << endl;
    //m_particles.addEffect(ParticleEffect::createBulletTail(bullet->getPosition(), bullet->direction(), bullet->lifetime()));
    m_bullets.insert(std::pair<int, Bullet::Ptr >(bullet->get_id(), bullet));
}

bool PhysicsEngine::process(int elapsed_time)
{
    map<int, Hittable::Ptr>::iterator h_it;
    map<int, PhysicalObject::Ptr>::iterator p_it;
    map<int, Bullet::Ptr>::iterator b_it;

    bool gameOver = false;
    if (m_hittables.size() == 1)
    {
        gameOver = true;
    }

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
        b->run(elapsed_time);

        // Check for collisions with present objects
        /**p_it = m_objects.begin();
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

            } else {
                p_it++;
            }
        }

        h_it = m_hittables.begin();
        while (h_it != m_hittables.end())
        {
            if (b->get_shooter_id() != (*h_it).second->getId() && (*h_it).second->hit(*b))
            {
                std::cout << "Treffer an Spieler " << (*h_it).second->getId() << std::endl;
                b->destroy();
                if (m_hittables.size() > 1)
                {
                    (*h_it)->setHealth((*h_it)->getHealth() - 1);
                    if ((*h_it)->getHealth() == 0)
                    {
                        m_particles.addEffect(ParticleEffect::createExplosionSphere((*h_it)->getPosition()));
                        h_it = m_hittables.erase(h_it);
                    }
                }
            }
            h_it++;
        }*/

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

    return gameOver;
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

void PhysicsEngine::process_collisions(int id_one, int id_two)
{
    check_id_type(id_one);
    check_id_type(id_two);
    m_particles.update();
}

void PhysicsEngine::check_id_type(int id_to_check)
{
    //TODO add particles by collision
    if((id_to_check >> 24) == 0)
    {
        m_objects.erase(id_to_check);
    } else
    {
        if((id_to_check & 0xFFFFFF) != 0)
        {

            if(m_bullets.count(id_to_check) == 1) {
                m_bullets[id_to_check]->destroy();
                m_bullets.erase(id_to_check);
            }
        }else
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
