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
#include "physics/PhysicalObject.hpp"

#include <thread>
#include <chrono>
#include <memory>

namespace asteroids
{

/**
 * @brief Renders a Bullet
 */
class Bullet: public PhysicalObject
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
    Bullet(const Vector3f& fighter_position, const Vector3f fighter_axis);

    ~Bullet() = default;

    /**
     * @brief Moves the bullet until it's lifetime is over.
     */
	void run();
    
    /**
     * @brief Renders the bullet via glutSolidSphere.
     */
	void render() override;

    /// Returns the radius of the bullet
    int radius();

    static int lifetime() { return m_lifetime;}

    Vector3f direction() {return m_fighterAxis;}
private:

    // Lifetime, i.e., how many timesteps the bullet visible
	static const int m_lifetime = 80;

	// True, if the bullet's lifetime isn't over yet
	bool m_alive;

	// Flight direction of the bullet
    Vector3f m_fighterAxis;

    /// Sphere objet to render the bullet
    Sphere m_sphere;

    size_t m_iterations;
};

} // namespace asteroids


#endif /* BULLET_HPP_ */
