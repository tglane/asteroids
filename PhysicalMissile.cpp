/**
 * PhysicalMissile.cpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#include "PhysicalMissile.hpp"
#include "io/TriangleMeshFactory.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

const float PhysicalMissile::maxDistance = 2000;
const float PhysicalMissile::fieldOfView = 1;
const float PhysicalMissile::speed = 25;
const float PhysicalMissile::rotSpeed = speed * 0.001f;

PhysicalMissile::PhysicalMissile(int id, Hittable::Ptr& target) : m_id(id), m_target(target), m_lifetime(1000) {}

void PhysicalMissile::render() {}

void PhysicalMissile::run(int elapsed_time)
{
    // decide, whether or not the target is detected by the missile
    bool targetDetected;
    // target has to be close enough
    Vector3f relPos = m_target->getPosition() - m_position;
    targetDetected = magnitude(relPos) < maxDistance;
    if (targetDetected)
    {
        // target has to be in the field of view
        float projectedDistance = (relPos * m_xAxis) / (m_xAxis * m_xAxis);
        float radius = projectedDistance * fieldOfView;
        float deviation = magnitude(relPos - m_xAxis * projectedDistance);
        targetDetected = deviation < radius;
    }

    // fly
    if (targetDetected)
    {
        // get rotation axis
        // the rotation axis lies on the y z plane of the missile, it doesn't have to roll
        // the portions of y and z are proportional to the side lengths of the cuboid between the missile and the target
        // in the local oriented coordinate system
        // the y portion is proportional to the z side length and vice versa because the longer the side length is the
        // more rotating in that direction is required and therefore less portion of that direction goes into the
        // rotation axis
        float x = signedDistanceToPlane(relPos, m_xAxis, m_yAxis, m_zAxis);
        float y = signedDistanceToPlane(relPos, m_yAxis, m_xAxis, m_zAxis);
        float z = signedDistanceToPlane(relPos, m_zAxis, m_xAxis, m_yAxis);
        Vector3f rotAxis = m_zAxis * (y / x) - m_yAxis * (z / x);
        // get rotation angle
        // the rotation angle is simply the angle between the missiles x axis and the relative position of the target
        float angle = acos((relPos * m_xAxis) / (magnitude(relPos) * magnitude(m_xAxis)));
        // rotate
        if (angle > 0)
        {
            // the rotation is done with a quaternion
            Quaternion q;
            q.fromAxis(rotAxis, (angle < rotSpeed) ? angle : rotSpeed);
            m_xAxis = q * m_xAxis;
            m_yAxis = q * m_yAxis;
            m_zAxis = q * m_zAxis;
        }
    }
    // move
    move(Transformable::FORWARD, speed * 60 * elapsed_time / 1000.0f);

    if (alive())
    {
        m_lifetime--;
    }
}

float PhysicalMissile::magnitude(Vector3f v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float PhysicalMissile::signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2)
{
    // orthogonal projection
    Vector3f p = e1 * ((x * e1) / (e1 * e1)) + e2 * ((x * e2) / (e2 * e2));
    // unsigned distance
    float d = magnitude(x - p);
    // signed distance
    if (magnitude(x + b) < magnitude(x - b))
    {
        d *= -1;
    }
    return d;
}

int PhysicalMissile::getId()
{
    return m_id;
}

bool PhysicalMissile::alive()
{
    return m_lifetime > 0;
}

void PhysicalMissile::destroy()
{
    m_lifetime = 0;
}

void PhysicalMissile::setShooterId(int shooterId)
{
    m_shooterId = shooterId;
}

int PhysicalMissile::getShooterId()
{
    return m_shooterId;
}

}
