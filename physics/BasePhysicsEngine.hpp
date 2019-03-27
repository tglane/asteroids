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


#ifndef BASEPHYSICSENGINE_HPP_
#define BASEPHYSICSENGINE_HPP_

#include <map>
#include <memory>

#include "PhysicalObject.hpp"
#include "PhysicalBullet.hpp"
#include "Hittable.hpp"


using std::map;


namespace asteroids
{

/**
 * @brief   Class that calculates the position of all present objects
 *          and implements collision detection. Handles particle
 *          effects like explosions.
 */
class BasePhysicsEngine
{
public:

    using Ptr = std::shared_ptr<BasePhysicsEngine>;

    /**
     * @brief   Ctor.
     */
    BasePhysicsEngine() = default;

    /**
     * @brief   Dtor.
     */
    virtual ~BasePhysicsEngine() = default;

    /**
     * @brief   Adds a destroyable objects, i.e. a static objects
     *          that can be hit by a bullet (asteroids etc.). Takes ownership of 
     *          the given pointer
     */
    void addDestroyable(PhysicalObject::Ptr d);

    void addHittable(Hittable::Ptr h);

    /**
     * @brief   Adds a bullet to the scene. Takes ownership of the given pointer
     */
    void addBullet(PhysicalBullet::Ptr bullet);

    /**
     * @brief   The engine's main loop
     */
    bool process(int elapsed_time);

protected:

    /// List of destroyable objects
    map<int, PhysicalObject::Ptr>    m_objects;

    /// List of active bullets
    map<int, PhysicalBullet::Ptr>            m_bullets;

    map<int, Hittable::Ptr>          m_hittables;
};

} /* namespace asteroids */
#endif /* BASEPHYSICSENGINE_HPP_ */
