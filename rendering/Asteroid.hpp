/*
 *  Asteroid.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */
#ifndef ASTEROID_HPP_
#define ASTEROID_HPP_

// Asteroid Project includes
#include "TexturedMesh.hpp"
#include "io/TriangleMeshFactory.hpp"
#include "physics/Transformable.hpp"
#include "physics/PhysicalObject.hpp"

// Std C++ includes
#include <cstdlib>
#include <memory>

namespace asteroids
{
/**
 * @brief  Representation of an asteroid
 */
class Asteroid: public PhysicalObject
{
public:
    using Ptr = std::shared_ptr<Asteroid>; 

    /**
     * @brief   Ctor.
     * @param   start_position   Position of the fighter that shoots this Asteroid
     * @param   scale            Scaling factor to vary the asteroid's size
     */
    Asteroid(TexturedMesh::Ptr mesh, 
             Vector3f startDirection, 
             Vector3f startPosition,
             float mass,
             float rotation,
             float speed,
             float accell,
             float radius);

    Asteroid(const Asteroid& other);

    Asteroid& operator=( Asteroid& rhs );

    /**
     * @brief   Ctor.
     */
    Asteroid() = delete;

    virtual ~Asteroid() = default;

    /**
     * @brief   Renders the asteroid
     */
    virtual void render() override;

    constexpr static float MODEL_RADIUS = 100;

private:
    void copy(const Asteroid& other);

    float  m_scale;

};

}
#endif /* ASTEROID_HPP_ */
