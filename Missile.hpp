#ifndef CPP18_ASTEROIDS_MISSILE_HPP
#define CPP18_ASTEROIDS_MISSILE_HPP

#include <string>
#include "physics/Transformable.hpp"
#include "physics/Hittable.hpp"
#include "rendering/Renderable.hpp"
#include "rendering/TriangleMesh.hpp"

namespace asteroids
{

class Missile : public Transformable, public Renderable
{

public:

    using Ptr = std::shared_ptr<Missile>;

    explicit Missile(Hittable::Ptr& target);

    void render() override;

    void fixMissile();

    void run(int elapsed_time);

private:

    float length(Vector3f v);

    TriangleMesh::Ptr m_mesh;

    Hittable::Ptr m_target;

};

}

#endif
