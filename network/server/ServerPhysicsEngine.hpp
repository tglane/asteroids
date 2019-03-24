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
#include <physics/BasePhysicsEngine.hpp>
#include <physics/Hittable.hpp>


using std::list;


namespace asteroids
{

/**
 * @brief   Class that calculates the position of all present objects
 *          and implements collision detection. Handles particle
 *          effects like explosions.
 */
class ServerPhysicsEngine: public BasePhysicsEngine
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

    std::list<std::pair<int, int>> detect_collisions();

private:

};

}/* namespace asteroids */
#endif /* PHYSICSENGINE_HPP_ */
