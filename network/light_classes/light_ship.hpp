//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_SHIP_HPP
#define CPP18_ASTEROIDS_LIGHT_SHIP_HPP

#include <vector>

#include "light_object.hpp"

class light_ship : public light_object {

public:

    light_ship(int id, int speed, int life, int shield, std::vector<float> position, std::vector<float> rotation)
        : m_id(id), m_speed(speed), m_position(position), m_direction(rotation), m_life(life), m_shield(shield)

    int get_life() { return m_life; }

    int get_shield() { return m_shield; }

    std::vector<float> get_position() { return m_position; }

private:

    int m_life;

    int m_shield;

};

#endif //CPP18_ASTEROIDS_LIGHT_SHIP_HPP
