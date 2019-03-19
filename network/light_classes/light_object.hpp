//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
#define CPP18_ASTEROIDS_LIGHT_OBJECT_HPP

#include <vector>
#include <math/Vector.hpp>

class light_object {

public:

    void move();

    int get_id() { return m_id; }

    void set_position(asteroids::Vector3f position) {m_position = position;}
    void set_rotation(asteroids::Vector3f rotation) {m_rotation = rotation;}
    void set_velocity(asteroids::Vector3f velocity) {m_velocity = velocity;}

protected:

    int m_id;

    uint32_t m_speed;

    asteroids::Vector3f m_position;
    asteroids::Vector3f m_rotation;
    asteroids::Vector3f m_velocity;

};


#endif //CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
