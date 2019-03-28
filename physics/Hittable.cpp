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
    float length = 200;
    float width = 100;
    float height = 50;

    Vector3f xp = this->getPosition() + this->getXAxis() * length;
    Vector3f xn = this->getPosition() - this->getXAxis() * length;
    Vector3f yp = this->getPosition() + this->getYAxis() * width;
    Vector3f yn = this->getPosition() - this->getYAxis() * width;
    Vector3f zp = this->getPosition() + this->getZAxis() * height;
    Vector3f zn = this->getPosition() - this->getZAxis() * height;

    float boxMaxX = signedDistanceToPlane(xp, this->getXAxis(), this->getYAxis(), this->getZAxis());
    float boxMinX = signedDistanceToPlane(xn, this->getXAxis(), this->getYAxis(), this->getZAxis());
    float boxMaxY = signedDistanceToPlane(yp, this->getYAxis(), this->getXAxis(), this->getZAxis());
    float boxMinY = signedDistanceToPlane(yn, this->getYAxis(), this->getXAxis(), this->getZAxis());
    float boxMaxZ = signedDistanceToPlane(zp, this->getZAxis(), this->getXAxis(), this->getYAxis());
    float boxMinZ = signedDistanceToPlane(zn, this->getZAxis(), this->getXAxis(), this->getYAxis());

    float bx = signedDistanceToPlane(position, this->getXAxis(), this->getYAxis(), this->getZAxis());
    float by = signedDistanceToPlane(position, this->getYAxis(), this->getXAxis(), this->getZAxis());
    float bz = signedDistanceToPlane(position, this->getZAxis(), this->getXAxis(), this->getYAxis());

    float x = std::max(boxMinX, std::min(bx, boxMaxX));
    float y = std::max(boxMinY, std::min(by, boxMaxY));
    float z = std::max(boxMinZ, std::min(bz, boxMaxZ));
    return magnitude(Vector3f(x - bx, y - by, z - bz)) < radius;
}

float Hittable::signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2)
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
