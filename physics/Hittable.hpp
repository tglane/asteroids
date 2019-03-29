/**
 * Hittable.hpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#ifndef CPP18_ASTEROIDS_HITTABLE_HPP
#define CPP18_ASTEROIDS_HITTABLE_HPP

#include <QtCore/QTime>
#include <rendering/Asteroid.hpp>
#include "rendering/Bullet.hpp"
#include "physics/PhysicalBullet.hpp"
#include "Transformable.hpp"

namespace asteroids {

/**
 * Class that represents something that can be hit in the game and has health,
 * namely the player camera and the enemy spacecraft
 */
class Hittable : public Transformable
{

public:

    /**
     * @brief Constructor
     */
    Hittable() = default;

    /**
     * @brief Constructor with initialization of the id
     * @param id
     */
    Hittable(int id): m_id(id) {}

    /// shared pointer to a hittable object
    using Ptr = std::shared_ptr<Hittable>;

    /**
     * @brief sets the id
     * @param id
     */
    void setId(int id);

    /**
     * @brief returns the id
     * @return id
     */
    int getId() const;

    /**
     * @brief sets the health
     * @param health
     */
    void setHealth(int health);

    /**
     * @brief returns the health
     * @return health
     */
    int getHealth();

    /**
     * @brief calculates whether or not the hittable collides with a bullet
     * @param b bullet
     * @return true if there is a collision otherwise false
     */
    bool hitBullet(PhysicalBullet& b);

    /**
     * @brief calculates whether or not the hittable collides with a physical object in the game namely an asteroid
     * @param p physical object
     * @return true if there is a collision otherwise false
     */
    bool hitAsteroid(PhysicalObject& p);

    /**
     * @brief calculates whether or not the hittable collides with a spherical object
     * the hitbox of the hittable is a cuboid with the size of the spaceship model that rotates with the player
     * @param position position of the sphere
     * @param radius radius of the sphere
     * @return true if there is a collision otherwise false
     */
    bool hit(Vector3f position, float radius);

    /**
     * @brief called when the hittable leaves the game space to start the timer
     */
    void outOfBound();

    /**
     * @brief returns the time
     * @return time
     */
    int getTime();

    /**
     * @brief called when the hittable is inside the game space
     */
    void inBound();

    /**
     * @brief restarts the timer with a given offset
     * @param mSecs offset
     */
    void restartTimer(int mSecs);

private:

    /**
     * @brief calculates the distance between a vector and a plane
     * calculates the signed distance of the vector x to the plane that is defined by the vectors e1 and e2
     * the vector b is used to get the sign, it points in the positive direction
     * @param x vector of which the distance to the plane is calculated
     * @param b vector that points in the positive direction
     * @param e1 vector that defines the plane
     * @param e2 vector that defines the plane
     * @return distance between vector and plane
     */
    static float signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2);

    /**
     * @brief magnitude of a vector
     * @param v vector of which the magnitude is calculated
     * @return magnitude
     */
    static float magnitude(Vector3f v);

    /// id of the hittable
    int m_id;

    /// health of the hittable
    int m_health;

    /// timer for out of bounds handling
    QTime m_timer;

    /// flag whether or not the hittable is out of the bounds of the game space
    bool m_isOutOfBound;

};

}

#endif
