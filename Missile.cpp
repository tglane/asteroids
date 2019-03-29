/**
 * Missile.hpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#include "Missile.hpp"

namespace asteroids
{

Missile::Missile(int id, asteroids::Hittable::Ptr &target) : PhysicalMissile(id, target)
{
    m_mesh = TriangleMeshFactory::instance().getMesh("../models/missile.3ds");
    m_mesh->fixMissile();
}

void Missile::render()
{
    computeMatrix();
    glPushMatrix();
    glMultMatrixf(m_transformation.getData());
    m_mesh->render();
    glPopMatrix();
}

}
