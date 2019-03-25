//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_ASTEROID_HPP
#define CPP18_ASTEROIDS_LIGHT_ASTEROID_HPP

#include <vector>

#include "light_object.hpp"

class light_asteroid : public light_object {

public:

    light_asteroid(int id, int speed, float size, std::vector<float> position, std::vector<float> direction)
            : m_id(id), m_speed(speed), m_size(size), m_position(position), m_direction(direction);

    float get_size() { return m_size; }

    std::vector<float> get_position() { return m_position; }

private:

    float m_size;

};


#endif //CPP18_ASTEROIDS_LIGHT_ASTEROID_HPP
