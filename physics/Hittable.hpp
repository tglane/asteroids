#ifndef CPP18_ASTEROIDS_HITTABLE_HPP
#define CPP18_ASTEROIDS_HITTABLE_HPP

#include "rendering/Bullet.hpp"
#include "Transformable.hpp"

namespace asteroids {

class Hittable : public Transformable
{

public:

    using Ptr = std::shared_ptr<Hittable>;

    void setId(int id);

    int getId() const;

    void setHealth(int health);

    bool hit(Bullet b);

private:

    static float signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2);

    static float magnitude(Vector3f v);

    int m_id;

    int m_health;

};

}

#endif
