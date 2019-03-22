/*
 *  PhsicalObject.cpp
 *
 *  @date 07.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include <cmath>

#include "PhysicalObject.hpp"
#include "math/Vector.hpp"
#include "math/Normal.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>         
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

PhysicalObject::PhysicalObject(Vector<float> direction, Vector<float> position, float mass,
                               float momentum, float speed, float accel, float radius, int id)
{
    this->m_accel = accel;
    this->m_position = position;
    this->m_speed = speed;
    this->m_mass = mass;
    this->m_momentum = momentum;
    this->m_radius = radius;
    this->m_alive = true;
    this->m_angle=0.01f;
    this->m_id = id;
}

PhysicalObject::PhysicalObject() :
    m_euler(0, 0, 0),
    m_momentum(0),
    m_mass(0),
    m_accel(0),
    m_radius(0),
    m_angle(0),
    m_alive(true),
    m_id(0)
{

}

PhysicalObject::PhysicalObject(int id) :
    m_euler(0, 0, 0),
    m_momentum(0),
    m_mass(0),
    m_accel(0),
    m_radius(0),
    m_angle(0),
    m_alive(true),
    m_id(id)
{

}

void PhysicalObject::destroy()
{
    m_alive = false;
}

bool PhysicalObject::collision(PhysicalObject* p)
{
    using std::pow;
    Vector<float> dist = m_position - p->m_position;
    return (pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2)) <= pow((m_radius + p->m_radius), 2);
}

bool PhysicalObject::collision(Vector<float> p, float r)
{
    using std::pow;
    Vector<float> dist = m_position - p;

    return (pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2)) <= pow((m_radius + r), 2);
}


/**
 * multiply the speed by factor
 */
void PhysicalObject::multiplySpeed(int factor)
{
    m_speed *= factor;
}

bool PhysicalObject::alive()
{
    return m_alive;
}

}
