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

#include "math/Vector.hpp"

#define PI 3.14159265
#define PH 1.57079632

namespace asteroids
{


/**
 * @brief Class to represent a virtual camera using gluLookAt
 * 
 */
class Camera
{
public:
    /**
     * @brief Enumeration to encode types of camera movements
     */
    enum CameraMovement 
    {
	    FORWARD,
	    BACKWARD,
	    LEFT,
	    RIGHT,
	    UP,
	    DOWN
    };

    /**
     * @brief Construct a new Camera object at (0, 0, 0) with
     *        upward orientation and lookAt at (0, 0, -1)
     * 
     */
    Camera();

    /**
     * @brief Construct a new Camera object with upward orientation
     *        and lookAt at (0, 0, -1)
     * 
     * @param position      Initial position
     * @param turnSpeed     Turning speed in radians per call
     * @param moveSpeed     Move speed in world units per call
     */
    Camera(const Vector3f& position, const float& turnSpeed, const float& moveSpeed);

    /**
     * @brief Moves the camera according to given direction
     * 
     * @param dir           Moving direction
     */
    void move(const CameraMovement& dir);

     /**
     * @brief turns the camera according to given direction
     * 
     * @param dir           Moving direction
     */
    void turn(const CameraMovement& dir);

    /**
     * @brief Calls gluLookAt with the internal parameters
     * 
     */
    void apply();
	void applyRotationOnly();

    /**
     * @brief Set the turn speed  of the camera
     * 
     * @param speed         The new turn speed
     */
    void setTurnSpeed(const float& speed) { m_turnSpeed = speed;}

    /**
     * @brief Set the move speed  of the camera
     * 
     * @param speed         The new move speed
     */
    void setMoveSpeed(const float& speed) { m_moveSpeed = speed;}
    
private:
    /// View up vector
    Vector3f  m_up;

    /// Translation
    Vector3f  m_trans;

    /// Look at Vector
    Vector3f  m_l;

    /// Rotation angles encoded in Vector, i.e., x is the
    /// rotation around the x-axis and so on
    Vector3f  m_rot;

    /// Initial position of the camera
    Vector3f  m_initial;

    /// Turn speed in radians per call
    float   m_turnSpeed;

    /// Move speed in world units per call
    float   m_moveSpeed;
};

} // namespace asteroids

#endif
