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


#ifndef SERVER_PHYSICSENGINE_HPP_
#define SERVER_PHYSICSENGINE_HPP_

#include <list>
#include <memory>
#include <map>

#include <physics/PhysicalObject.hpp>
#include <physics/PhysicalBullet.hpp>
#include <physics/PhysicalSpaceCraft.hpp>


using std::list;


namespace asteroids
{

/**
 * @brief   Class that calculates the position of all present objects
 *          and implements collision detection. Handles particle
 *          effects like explosions.
 */
class ServerPhysicsEngine
{
public:

    using Ptr = std::shared_ptr<ServerPhysicsEngine>;

    /**
     * @brief   Ctor.
     */
    ServerPhysicsEngine() = default;

    /**
     * @brief   Dtor.
     */
    virtual ~ServerPhysicsEngine() = default;

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
    void addBullet(PhysicalBullet::Ptr& bullet);

    bool gameOver();
    /**
     * @brief   The engine's main loop
     */
    std::list<std::pair<int, int>> process(int time_elapsed);

private:

    /// List of destroyable objects
    map<int, PhysicalObject::Ptr>    m_objects;

    /// List of active bullets
    map<int, PhysicalBullet::Ptr>     m_bullets;

    map<int, Hittable::Ptr>          m_hittables;

};

}/* namespace asteroids */
#endif /* PHYSICSENGINE_HPP_ */
