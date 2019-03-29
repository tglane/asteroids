
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
#include <QtCore/QFileInfo>

using namespace std;

namespace asteroids
{
    PhysicsEngine::PhysicsEngine()
    {
        m_explosionSound.setSource(QUrl::fromLocalFile(QFileInfo("../models/explosion.wav").absoluteFilePath()));
        m_explosionSound.setVolume(0.6f);

        m_hitmarkerSound.setSource(QUrl::fromLocalFile(QFileInfo("../models/hitmarker.wav").absoluteFilePath()));
        m_hitmarkerSound.setVolume(0.7f);
    }

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
        map<int, Missile::Ptr>::iterator m_it;

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

        m_it = m_missiles.begin();
        while(m_it != m_missiles.end())
        {
            Missile::Ptr m = (*m_it).second;
            m->render();
            m_it++;
        }

        m_particles.render();

    }

void PhysicsEngine::process_collisions(int id_one, int id_two)
{
    int first = check_id_type(id_one, -1);
    int second = check_id_type(id_two, first);
    m_particles.update();

    /* Add life to player when he hits an asteroid */
    if(first == 1 && second == 0)
    {
        int shooter_id = id_one & 0xFF000000;
        if(m_hittables.count(shooter_id) == 1)
        {
            int health = m_hittables[shooter_id]->getHealth();
            if (health % 10 != 0) {
                m_hittables[shooter_id]->setHealth(health + 1);
            }
        }
    }
}

int PhysicsEngine::check_id_type(int id_to_check, int prevId)
{
    if((id_to_check >> 24) == 0)
    {
        /* asteroid collision */
        if(m_objects.count(id_to_check == 1))
        {
            m_particles.addEffect(ParticleEffect::createExplosionSphere(m_objects[id_to_check]->getPosition()));
            m_objects.erase(id_to_check);
        }
        return 0;
    }
    else if((id_to_check & 0xFFFFFF) != 0)
    {
        if (((id_to_check >> 16) & 0x000000FF) == 0)
        {
            /* Bullet collision */
            if(m_bullets.count(id_to_check) == 1) {

                m_bullets[id_to_check]->destroy();
                m_bullets.erase(id_to_check);
            }
            return 1; // Means Bullet
        }
        else if (((id_to_check >> 16) & 255) == 1)
        {
            /* Missile collision */
            if (m_missiles.count(id_to_check) == 1) {
                m_missiles.erase(id_to_check);
            }
            return 3; // Means Missile
        }
    }
    else {
        /* Player collision */
        if (m_hittables.count(id_to_check) == 1) {

            m_hitmarkerSound.play();

            int health = m_hittables[id_to_check]->getHealth();
            if (prevId == 3) {
                int shipHealth = health % 10;
                m_hittables[id_to_check]->setHealth(health - ((shipHealth == 1 || shipHealth == 2) ? shipHealth : 3));
            } else {
                m_hittables[id_to_check]->setHealth(health - 1);
            }
            health = m_hittables[id_to_check]->getHealth();
            if (health % 10 == 0) {
                m_explosionSound.play();

                m_particles.addEffect(ParticleEffect::createExplosionSphere(m_hittables[id_to_check]->getPosition()));
                m_particles.addEffect(ParticleEffect::createExplosionRing(m_hittables[id_to_check]->getPosition()));
            }
            if (health == 0) {
                m_hittables.erase(id_to_check);
            }
        }
        return 2; // Means Player
    }
    return -1;
}

void PhysicsEngine::reset_lists()
    {
        m_hittables.clear();
        m_objects.clear();
        m_bullets.clear();
        m_missiles.clear(); // wild guess :D
        m_particles.reset_particles();
    }

} /* namespace asteroids */
