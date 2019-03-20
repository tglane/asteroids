/*
 *  Bullet.hpp
 *
 *  @date 19.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#ifndef BULLET_HPP_
#define BULLET_HPP_

#include "TriangleMesh.hpp"
#include "Sphere.hpp"
#include "physics/PhysicsBullet.hpp"

#include <thread>
#include <chrono>
#include <memory>

namespace asteroids
{

/**
 * @brief Renders a Bullet
 */
class Bullet: public Renderable, public PhysicsBullet
{

public:

    using Ptr = std::shared_ptr<Bullet>;

    /**
     * @brief Contructor. Build a bullet on the given Fighter's
     *                      position. The bullet will move on the
     *                      given axis.
     * @param   fighter_position   Position of the fighter that shoots this bullet
     * @param   fighter_axis   Axis the bullet will move on
     */
    Bullet(const Vector3f& fighter_position, const Vector3f fighter_axis)
        : PhysicsBullet(fighter_position, fighter_axis) {}

    ~Bullet() = default;

    /**
     * @brief Renders the bullet via glutSolidSphere.
     */
	void render() override;
private:
    Sphere m_sphere;

};

} // namespace asteroids


#endif /* BULLET_HPP_ */
