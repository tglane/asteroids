#ifndef CPP18_ASTEROIDS_MISSILE_HPP
#define CPP18_ASTEROIDS_MISSILE_HPP

#include "PhysicalMissile.hpp"

namespace asteroids
{

class Missile : public PhysicalMissile
{

public:

    using Ptr = std::shared_ptr<PhysicalMissile>;

    Missile(int id, Hittable::Ptr& target);

    void render() override;

private:

    TriangleMesh::Ptr m_mesh;

};

}

#endif
