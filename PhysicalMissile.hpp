/**
 * PhysicalMissile.hpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#ifndef CPP18_ASTEROIDS_PHYSICALMISSILE_HPP
#define CPP18_ASTEROIDS_PHYSICALMISSILE_HPP

#include <string>
#include "physics/Transformable.hpp"
#include "physics/Hittable.hpp"
#include "rendering/Renderable.hpp"
#include "rendering/TriangleMesh.hpp"

namespace asteroids
{

/**
 * Class for an automatically target chasing missile that only contains its physical properties
 * so that it can be used by the server
 */
class PhysicalMissile : public Transformable, public Renderable
{

public:

    /// shared pointer to a physical missile
    using Ptr = std::shared_ptr<PhysicalMissile>;

    /**
     * @brief Constructor
     * @param id id of the missile
     * @param target target that gets chased by the missile
     */
    PhysicalMissile(int id, Hittable::Ptr& target);

    /**
     * @brief does nothing because a physical missile cannot be rendered
     */
    void render() override;

    /**
     * @brief chases the target if it is in sight otherwise moves forward
     * @param elapsed_time the time that has passed since the last call used to prevent depending on the framerate of the game
     */
    void run(int elapsed_time);

    /**
     * @brief returns the id
     * @return id
     */
    int getId();

    /**
     * @brief returns whether or not the missile is still alive
     * @return true if the missile is alive otherwise false
     */
    bool alive();

    /**
     * @brief sets the missiles lifetime to 0
     */
    void destroy();

    /**
     * @brief sets the id of the missiles shooter
     * @param shooterId id of the shooter of the missile
     */
    void setShooterId(int shooterId);

    /**
     * @brief returns the id of the missiles shooter
     * @return id of the shooter of the missile
     */
    int getShooterId();

private:

    /// maximum distance that the target can be away and be chased
    static const float maxDistance;
    /// radius of the field of view cone with height 1
    static const float fieldOfView;
    /// speed of the missile
    static const float speed;
    /// rotation speed of the missile
    static const float rotSpeed;

    /// id of the missile
    int m_id;

    /// id of the shooter of the missile
    int m_shooterId;

    /// target of the missile
    Hittable::Ptr m_target;

    /// remaining lifetime of the missile
    int m_lifetime;

    /**
     * @brief magnitude of a vector
     * @param v vector of which the magnitude is calculated
     * @return magnitude
     */
    float magnitude(Vector3f v);

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
    float signedDistanceToPlane(Vector3f x, Vector3f b, Vector3f e1, Vector3f e2);

};

}

#endif
