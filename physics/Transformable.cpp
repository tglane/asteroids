/*
 *  Transformable.cpp
 *
 *  @date 07.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Transformable.hpp"

#include <iostream>
using namespace std;

namespace asteroids
{

Transformable::Transformable()
{
    // Setup rotation and position
    m_xAxis         = Vector<float>(1.0, 0.0, 0.0);
    m_yAxis         = Vector<float>(0.0, 1.0, 0.0);
    m_zAxis         = Vector<float>(0.0, 0.0, 1.0);
    m_position      = Vector<float>(0.0, 0.0, 0.0);

    // Set tranformation matrix to unit matrix
    for(int i = 0; i < 16; i++)
    {
        m_transformation[i] = 0.0f;
    }
    m_transformation[0 ] = 1.0f;
    m_transformation[5 ] = 1.0f;
    m_transformation[10] = 1.0f;
    m_transformation[15] = 1.0f;
}

void Transformable::move()
{
    m_position = m_position + m_xAxis * m_speed;
}

void Transformable::move(MoveTransform axis)
{
   move(axis, m_speed);
}


void Transformable::rotate(RotationTransform axis)
{
    rotate(axis, m_rotationSpeed);
}

void Transformable::rotate(RotationTransform axis, float s)
{

    Quaternion nq;

    // Get the wanted operation and calculate the new coordinates
    switch(axis)
    {
        case ROLL_CLOCKWISE:
            nq.fromAxis(m_xAxis, s);
            m_yAxis = nq * m_yAxis;
            m_zAxis = nq * m_zAxis;
            break;
        case ROLL_COUNTERCLOCKWISE:
            nq.fromAxis(m_xAxis, -s);
            m_yAxis = nq * m_yAxis;
            m_zAxis = nq * m_zAxis;
            break;

        case PITCH_DOWN:
            nq.fromAxis(m_yAxis, s);
            m_xAxis = nq * m_xAxis;
            m_zAxis = nq * m_zAxis;
            break;
        case PITCH_UP:
            nq.fromAxis(m_yAxis, -s);
            m_xAxis = nq * m_xAxis;
            m_zAxis = nq * m_zAxis;
            break;

        case YAW_COUNTERCLOCKWISE:
            nq.fromAxis(m_zAxis, s);
            m_xAxis = nq * m_xAxis;
            m_yAxis = nq * m_yAxis;
            break;
        case YAW_CLOCKWISE:
            nq.fromAxis(m_zAxis, -s);
            m_xAxis = nq * m_xAxis;
            m_yAxis = nq * m_yAxis;
            break;
    }
}

void Transformable::move(MoveTransform axis, float speed)
{
    Vector<float> direction;

    // Determine the correct axis of the local system
    switch(axis)
    {
        case FORWARD:
            direction = m_xAxis;
            break;
        case BACKWARD:
            direction = m_xAxis * -1.0;
            break;
        case STRAFE_LEFT:
            direction = m_yAxis;
            break;
        case STRAFE_RIGHT:
            direction = m_yAxis * -1.0;
            break;
        case LIFT_UP:
            direction = m_zAxis;
            break;
        case LIFT_DOWN:
            direction = m_zAxis * -1.0;
            break;
    }

    // Update mesh position
    m_position = m_position + direction * speed;
}

void Transformable::computeMatrix()
{
    // Compute the transformation matrix for this object
    // according to the current position and rotation
    // state
    m_transformation[0]  = m_yAxis[0];
    m_transformation[1]  = m_yAxis[1];
    m_transformation[2]  = m_yAxis[2];

    m_transformation[4]  = m_xAxis[0];
    m_transformation[5]  = m_xAxis[1];
    m_transformation[6]  = m_xAxis[2];

    m_transformation[8]  = m_zAxis[0];
    m_transformation[9]  = m_zAxis[1];
    m_transformation[10] = m_zAxis[2];

    m_transformation[12] = m_position[0];
    m_transformation[13] = m_position[1];
    m_transformation[14] = m_position[2];


}

void Transformable::setXAxis(const Vector<float> &m_xAxis) {
    Transformable::m_xAxis = m_xAxis;
}

void Transformable::setYAxis(const Vector<float> &m_yAxis) {
    Transformable::m_yAxis = m_yAxis;
}

void Transformable::setZAxis(const Vector<float> &m_zAxis) {
    Transformable::m_zAxis = m_zAxis;
}

}
