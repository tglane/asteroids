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

#include <list>
#include <memory>

#include "PhysicalObject.hpp"
#include "ParticleEngine.hpp"
#include "rendering/Bullet.hpp"
#include "rendering/SpaceCraft.hpp"


using std::list;


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
    void process();

private:

    /// List of destroyable objects
    list<PhysicalObject::Ptr>    m_objects;

    /// List of active bullets
    list<Bullet::Ptr>            m_bullets;

    ParticleEngine               m_particles;


};

} /* namespace asteroids */
#endif /* PHYSICSENGINE_HPP_ */
