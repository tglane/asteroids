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
const float PhysicalMissile::fieldOfView = 1; // radius of the field of view cone with height 1
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
        float x = signedDistanceToPlane(relPos, m_xAxis, m_yAxis, m_zAxis);
        float y = signedDistanceToPlane(relPos, m_yAxis, m_xAxis, m_zAxis);
        float z = signedDistanceToPlane(relPos, m_zAxis, m_xAxis, m_yAxis);
        Vector3f rotAxis = m_zAxis * (y / x) - m_yAxis * (z / x);
        // get rotation angle
        float angle = acos((relPos * m_xAxis) / (magnitude(relPos) * magnitude(m_xAxis)));
        // rotate
        if (angle > 0)
        {
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
    // Projection
    Vector3f p = e1 * ((x * e1) / (e1 * e1)) + e2 * ((x * e2) / (e2 * e2));
    // Unsigned distance
    float d = magnitude(x - p);
    // Signed distance
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

}
