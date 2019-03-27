#include "Missile.hpp"
#include "io/TriangleMeshFactory.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

const float Missile::maxDistance = 2000;
const float Missile::fieldOfView = 1; // radius of the field of view cone with height 1
const float Missile::speed = 25;
const float Missile::rotSpeed = speed * 0.001f;

Missile::Missile(Hittable::Ptr& target) : m_target(target)
{
    m_mesh = TriangleMeshFactory::instance().getMesh("../models/missile.3ds");
}

void Missile::render()
{
    computeMatrix();
    glPushMatrix();
    glMultMatrixf(m_transformation.getData());
    m_mesh->render();
    glPopMatrix();
}

void Missile::fixMissile()
{
    m_mesh->fixMissile();
}

void Missile::run(int elapsed_time)
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
        if (!(rotAxis[0] == 0 && rotAxis[1] == 0 && rotAxis[2] == 0))
        {
            // get rotation angle
            // TODO
            // rotate
            Quaternion q;
            q.fromAxis(rotAxis, rotSpeed);
            m_xAxis = q * m_xAxis;
            m_yAxis = q * m_yAxis;
            m_zAxis = q * m_zAxis;
        }
    }
    // move
    move(Transformable::FORWARD, speed * 60 * elapsed_time / 1000.0f);
}

float Missile::magnitude(Vector3f v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Missile::signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2)
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

}
