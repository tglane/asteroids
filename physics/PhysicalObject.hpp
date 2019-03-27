/*
 *  PhysicalObject.hpp
 *
 *  @date 07.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef PHYSICALOBJECT_HPP_
#define PHYSICALOBJECT_HPP_

#include "Transformable.hpp"
#include "rendering/Renderable.hpp"
#include "math/Vector.hpp"

#include <iostream>
#include <memory>

using namespace std;

namespace asteroids
{

/**
 * @brief   Object with pyhsical properties.
 */
class PhysicalObject : public Transformable, public Renderable
{
public:

    using Ptr = std::shared_ptr<PhysicalObject>;

    /**
     * @brief contructs a PhysicalObject
     * @param direction the course
     * @param position the position in spawn
     * @param mass the weight
     * @param momentum the rotation
     * @param speed the speed to direction
     * @param accel
     * @param radius the collision ball
     */
    PhysicalObject( 
        Vector<float> direction, 
        Vector<float> position, 
        float mass, float momentum, float speed, 
        float accel, float radius, int id);

    PhysicalObject(int id);
    PhysicalObject();

    /**
     * @brief destroys the object
     */
    virtual ~PhysicalObject() = default;

    /**
     * @brief destroy this object
     */
    virtual void destroy();

    virtual void render() {};

    /**
     * @brief multiply the speed by factor
     * @param factor the
     */
    void multiplySpeed(int factor);


    float getSpeed();
    /**
     * @brief checks of collision with a other object
     * @param p the other object, that could collision
     * @return true if collision is located
     * */
    bool collision(PhysicalObject* p);

    bool collision(Vector<float> v, float r);

    /**
     * @brief gives information of alive
     * @return true if alive
     * */
    bool alive();

    int get_id() { return m_id; };

    virtual int radius()
    {
        return (int) m_radius;
    }


protected:
    unsigned int id;


    /**
     * the rotation
     */
    Vector<float> m_euler;

    /**
     * the rotation
     */
    float m_momentum;

    /**
     * the weight
     */
    float m_mass;


    /**
     * the accel
     */
    float m_accel;

    /**
     * the radius
     */
    float m_radius;

    /**
     * the angle
     */
    float m_angle;

    /**
     * alive
     */
    bool m_alive;

    int m_id;

};

} // namespace asteroids

#endif /* PHYSICALOBJECT_HPP_ */
