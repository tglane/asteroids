/*
 *  Transformable.hpp
 *
 *  @date 07.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef TRANSFORMABLE_H_
#define TRANSFORMABLE_H_

#include "math/Quaternion.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"

namespace asteroids
{

/**
 * @brief   Base class for all objects that can be transformed, i.e.
 *          moved and rotated.
 */
class Transformable
{
public:
    enum MoveTransform {FORWARD, BACKWARD, STRAFE_LEFT, STRAFE_RIGHT, LIFT_UP, LIFT_DOWN};
    enum RotationTransform{YAW_COUNTERCLOCKWISE, YAW_CLOCKWISE, PITCH_DOWN, PITCH_UP, ROLL_CLOCKWISE, ROLL_COUNTERCLOCKWISE};

    Transformable();
    virtual ~Transformable() = default;

    /**
     * @brief   Rotates a triangle mesh at the given axis with the given speed
     * @param axis axis to rotate
     * @param speed speed of rotation
     */
    void rotate(RotationTransform axis, float speed);

    void rotate(RotationTransform axis);

    /**
     * @brief   Moves a triangle mesh at the given axis with the given speed
     * @param axis axis to move
     * @param speed speed of movement
     */
    virtual void move(MoveTransform axis, float speed);

    virtual void move(MoveTransform axis);

    virtual void move();

    /**
     * @brief gives the current position
     * @return the current position
     */
    virtual Vector<float> getPosition()
    {
       return m_position;
    }

    /**
     * @brief set a new position
     * @param pos the new position
     */
    virtual void setPosition(Vector<float> pos)
    {
        m_position = pos;
    }


    /**
     * @brief gives the current speed
     * @return the current speed
     */
    virtual float getSpeed()
    {
        return m_speed;
    }

    /**
     * @brief set a new speed
     * @param s the new speed
     */
    virtual void setSpeed(float s)
    {
        m_speed = s;
    }

    /**
     * @brief   Returns the orientation of the x in the global coordinate system
     */
    Vector<float> getXAxis() { return m_xAxis;}

    /**
     * @brief   Returns the orientation of the y in the global coordinate system
     */
    Vector<float> getYAxis() { return m_yAxis;}

    /**
     * @brief   Returns the orientation of the z in the global coordinate system
     */
    Vector<float> getZAxis() { return m_zAxis; }

    /**
     * @brief sets the x axis
     * @param m_xAxis
     */
    void setXAxis(const Vector<float> &m_xAxis);

    /**
     * @brief sets the y axis
     * @param m_yAxis
     */
    void setYAxis(const Vector<float> &m_yAxis);

    /**
     * @brief sets the z axis
     * @param m_zAxis
     */
    void setZAxis(const Vector<float> &m_zAxis);

protected:

    /**
     * @brief   Computes the 4x4 transformation matrix of this object (needed for OpenGL)
     */
    void computeMatrix();

    /**
     * @brief   The transformation matrix for this object
     */
    Matrix   m_transformation;

    /**
     * @brief   The current orientation of the x axis
     */
    Vector<float>    m_xAxis;

    /**
     * @brief   The current orientation of the y axis
     */
    Vector<float>    m_yAxis;

    /**
     * @brief   The current orientation of the z axis
     */
    Vector<float>    m_zAxis;


    /**
     * @brief   The current position of the mesh object
     */
    Vector<float>    m_position;

    /**
     * @brief   The current moving speed
     */
    float           m_speed;

    float           m_rotationSpeed;
};

}

#endif /* TRANSFORMABLE_H_ */
