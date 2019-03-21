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


#ifndef PHYSICS_BULLET_HPP_
#define PHYSICS_BULLET_HPP_

#include "physics/PhysicalObject.hpp"

#include <thread>
#include <chrono>
#include <memory>

namespace asteroids
{

/**
 * @brief Renders a PhysicalBullet
 */
class PhysicalBullet: public PhysicalObject
{

public:

    using Ptr = std::shared_ptr<PhysicalBullet>;

    /**
     * @brief Contructor. Build a bullet on the given Fighter's
     *                      position. The bullet will move on the
     *                      given axis.
     * @param   fighter_position   Position of the fighter that shoots this bullet
     * @param   fighter_axis   Axis the bullet will move on
     */
    PhysicalBullet(const Vector3f& fighter_position, const Vector3f fighter_axis, int shooter_id);

    ~PhysicalBullet() = default;

    /**
     * @brief Moves the bullet until it's lifetime is over.
     */
	void run();
    
    /// Returns the radius of the bullet
    int radius();

    static int lifetime() { return m_lifetime;}

    Vector3f direction() {return m_fighterAxis;}

	int get_shooter_id() const;

private:

    // Lifetime, i.e., how many timesteps the bullet visible
	static const int m_lifetime = 80;

	// True, if the bullet's lifetime isn't over yet
	bool m_alive;

    float m_radius;

	// Flight direction of the bullet
    Vector3f m_fighterAxis;

    size_t m_iterations;

    int m_shooter_id;
};

} // namespace asteroids


#endif /* BULLET_HPP_ */
