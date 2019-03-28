#ifndef CPP18_ASTEROIDS_PHYSICALMISSILE_HPP
#define CPP18_ASTEROIDS_PHYSICALMISSILE_HPP

#include <string>
#include "physics/Transformable.hpp"
#include "physics/Hittable.hpp"
#include "rendering/Renderable.hpp"
#include "rendering/TriangleMesh.hpp"

namespace asteroids
{

class PhysicalMissile : public Transformable, public Renderable
{

public:

    using Ptr = std::shared_ptr<PhysicalMissile>;

    PhysicalMissile(int id, Hittable::Ptr& target);

    void render() override;

    void run(int elapsed_time);

    int getId();

    bool alive();

    void destroy();

    void setShooterId(int shooterId);

    int getShooterId();

private:

    static const float maxDistance;
    static const float fieldOfView;
    static const float speed;
    static const float rotSpeed;

    int m_id;

    int m_shooterId;

    float magnitude(Vector3f v);

    float signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2);

    Hittable::Ptr m_target;

    int m_lifetime;

};

}

#endif
