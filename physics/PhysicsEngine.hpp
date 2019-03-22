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


#ifndef PHYSICSENGINE_HPP_
#define PHYSICSENGINE_HPP_

#include <map>
#include <memory>

#include "PhysicalObject.hpp"
#include "ParticleEngine.hpp"
#include "rendering/Bullet.hpp"
#include "rendering/SpaceCraft.hpp"


using std::map;


namespace asteroids
{

/**
 * @brief   Class that calculates the position of all present objects
 *          and implements collision detection. Handles particle
 *          effects like explosions.
 */
class PhysicsEngine : public Renderable
{
public:

    using Ptr = std::shared_ptr<PhysicsEngine>;

    /**
     * @brief   Ctor.
     */
    PhysicsEngine() = default;

    /**
     * @brief   Dtor.
     */
    virtual ~PhysicsEngine() = default;

    /**
     * @brief   Adds a destroyable objects, i.e. a static objects
     *          that can be hit by a bullet (asteroids etc.). Takes ownership of 
     *          the given pointer
     */
    void addDestroyable(PhysicalObject::Ptr& d);

    void addHittable(Hittable::Ptr& h);

    /**
     * @brief   Adds a bullet to the scene. Takes ownership of the given pointer
     */
    void addBullet(Bullet::Ptr& bullet);

    /**
     * @brief   Renders all objects and particle effects
     */
    void render();

    /**
     * @brief   The engine's main loop
     */
    bool process(int elapsed_time);

    void process_collisions(int id_one, int id_two);
    void check_id_type(int id_to_check);

    /// Getter for current highest IDs
    int get_curr_bull_id() { return ++curr_bull_id; }
    int get_curr_dest_id() { return curr_dest_id; }

private:

    /// List of destroyable objects
    map<int, PhysicalObject::Ptr>    m_objects;

    /// List of active bullets
    map<int, Bullet::Ptr>            m_bullets;

    ParticleEngine               m_particles;

    map<int, Hittable::Ptr>          m_hittables;

    /// Current highest id of asteroids and bullets
    int curr_bull_id = 1;
    int curr_dest_id = 1;
    int curr_player_id = 1;

};

} /* namespace asteroids */
#endif /* PHYSICSENGINE_HPP_ */
