//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_BULLET_HPP
#define CPP18_ASTEROIDS_LIGHT_BULLET_HPP

#include <vector>

#include "light_object.hpp"

class light_bullet : public light_object {

public:

    light_bullet(int id, int speed, std::vector<float> position, std::vector<float> direction)
            : m_id(id), m_speed(speed), m_position(position), m_direction(direction)

    std::vector<float> get_position() { return m_position; }

};


#endif //CPP18_ASTEROIDS_LIGHT_BULLET_HPP
