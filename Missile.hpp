/**
 * Missile.hpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#ifndef CPP18_ASTEROIDS_MISSILE_HPP
#define CPP18_ASTEROIDS_MISSILE_HPP

#include "PhysicalMissile.hpp"

namespace asteroids
{

/**
 * Class for an automatically target chasing missile
 */
class Missile : public PhysicalMissile
{

public:

    /// shared pointer to a missile
    using Ptr = std::shared_ptr<PhysicalMissile>;

    /**
     * @brief Constructor
     * @param id id of the missile
     * @param target target that gets chased by the missile
     */
    Missile(int id, Hittable::Ptr& target);

    /**
     * @brief renders the missile
     */
    void render() override;

private:

    /// mesh of the missile
    TriangleMesh::Ptr m_mesh;

};

}

#endif
