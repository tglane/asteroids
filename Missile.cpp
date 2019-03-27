#include "Missile.hpp"
#include "io/TriangleMeshFactory.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

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

    float maxDistance = 2000;
    float fieldOfView = 0.5; // radius of the field of view cone with height 1

    // target has to be close enough
    Vector3f relPos = m_target->getPosition() - m_position;
    targetDetected = length(relPos) < maxDistance;
    if (targetDetected)
    {
        // target has to be in the field of view
        float projectedDistance = (relPos * m_xAxis) / (m_xAxis * m_xAxis);
        float radius = projectedDistance * fieldOfView;
        float deviation = length(relPos - m_xAxis * projectedDistance);
        targetDetected = deviation < radius;
    }

    // fly
    if (targetDetected)
    {

    }
    move(Transformable::FORWARD, 1000 * elapsed_time / 1000.0f);
}

float Missile::length(Vector3f v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

}
