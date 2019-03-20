//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
#define CPP18_ASTEROIDS_LIGHT_OBJECT_HPP

#include <vector>
#include <math/Vector.hpp>

class light_object {

protected:

    uint32_t m_id;

    uint32_t m_speed;

    asteroids::Vector3f m_position;
    asteroids::Vector3f m_velocity;
    asteroids::Quaternion m_rotation;


public:
    light_object(uint32_t id): m_id(id) {}

    void move();

    int get_id() { return m_id; }

    void set_position(asteroids::Vector3f position) {m_position = position; }
    void set_velocity(asteroids::Vector3f velocity) {m_velocity = velocity; }
    void set_rotation(asteroids::Quaternion rotation) {m_rotation = rotation; }

    const asteroids::Vector3f& get_position() { return m_position; }
    const asteroids::Vector3f& get_velocity() { return m_velocity; }
    const asteroids::Quaternion& get_rotation() { return m_rotation; }



    std::vector<float> m_direction;

};


#endif //CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
