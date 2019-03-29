/**
 * Hittable.cpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#include "Hittable.hpp"
#include <algorithm>
#include <rendering/Asteroid.hpp>

namespace asteroids
{

void Hittable::setId(int id)
{
    m_id = id;
}

int Hittable::getId() const
{
    return m_id;
}


void Hittable::setHealth(int health)
{
    m_health = health;
}

int Hittable::getHealth()
{
    return m_health;
}

bool Hittable::hitBullet(PhysicalBullet& b)
{
    return hit(b.getPosition(), b.radius());
}

bool Hittable::hitAsteroid(PhysicalObject& p)
{
    return hit(p.getPosition(), p.radius());
}

bool Hittable::hit(Vector3f position, float radius)
{
    // dimensions of the spaceship
    float length = 200;
    float width = 100;
    float height = 50;

    // outer points of the cuboid
    Vector3f xp = this->getPosition() + this->getXAxis() * length;
    Vector3f xn = this->getPosition() - this->getXAxis() * length;
    Vector3f yp = this->getPosition() + this->getYAxis() * width;
    Vector3f yn = this->getPosition() - this->getYAxis() * width;
    Vector3f zp = this->getPosition() + this->getZAxis() * height;
    Vector3f zn = this->getPosition() - this->getZAxis() * height;

    // coordinates of the outer points of the cuboid in the orientation of the local coordinate system
    float boxMaxX = signedDistanceToPlane(xp, this->getXAxis(), this->getYAxis(), this->getZAxis());
    float boxMinX = signedDistanceToPlane(xn, this->getXAxis(), this->getYAxis(), this->getZAxis());
    float boxMaxY = signedDistanceToPlane(yp, this->getYAxis(), this->getXAxis(), this->getZAxis());
    float boxMinY = signedDistanceToPlane(yn, this->getYAxis(), this->getXAxis(), this->getZAxis());
    float boxMaxZ = signedDistanceToPlane(zp, this->getZAxis(), this->getXAxis(), this->getYAxis());
    float boxMinZ = signedDistanceToPlane(zn, this->getZAxis(), this->getXAxis(), this->getYAxis());

    // coordinates of the sphere in the orientation of the local coordinate system
    float sx = signedDistanceToPlane(position, this->getXAxis(), this->getYAxis(), this->getZAxis());
    float sy = signedDistanceToPlane(position, this->getYAxis(), this->getXAxis(), this->getZAxis());
    float sz = signedDistanceToPlane(position, this->getZAxis(), this->getXAxis(), this->getYAxis());

    // coordinates of the closest point of the cuboid to the center of the sphere
    float x = std::max(boxMinX, std::min(sx, boxMaxX));
    float y = std::max(boxMinY, std::min(sy, boxMaxY));
    float z = std::max(boxMinZ, std::min(sz, boxMaxZ));
    // if the center of the sphere has a distance to the cuboid that is less than its radius there is a collision
    return magnitude(Vector3f(x - sx, y - sy, z - sz)) < radius;
}

float Hittable::signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2)
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

float Hittable::magnitude(Vector3f v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void Hittable::outOfBound()
{
    if (!m_isOutOfBound)
    {
        m_isOutOfBound = true;
        m_timer.start();
    }
}

int Hittable::getTime()
{
    return m_timer.elapsed();
}

void Hittable::inBound()
{
    m_isOutOfBound = false;
}

void Hittable::restartTimer(int mSecs)
{
    m_timer.start();
    m_timer = m_timer.addMSecs(-mSecs);
}

}
