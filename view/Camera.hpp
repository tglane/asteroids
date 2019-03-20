/*
 *  Camera.hpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "physics/Hittable.hpp"
#include "math/Vector.hpp"

namespace asteroids
{

/**
 * @brief Class to represent a virtual camera using gluLookAt
 * 
 */
class Camera : public Hittable
{
public:

    using Ptr = std::shared_ptr<Camera>;

    /**
     * @brief Construct a new Camera object at (0, 0, 0) with
     *        upward orientation and lookAt at (0, 0, -1)
     */
    Camera();

    /**
     * @brief Calls gluLookAt with the internal parameters
     */
    void apply();
};

}

#endif
