//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_SHIP_HPP
#define CPP18_ASTEROIDS_LIGHT_SHIP_HPP

#include <vector>

class light_ship {

public:

    light_ship(int id, int life, int shield, std::vector<float> position, std::vector<float> rotation)
        : m_id(id), m_position(position), m_rotation(rotation), m_life(life), m_shield(shield)

    int get_id() { return m_id; }

    int get_life() { return m_life; }

    int get_shield() { return m_shield; }

    std::vector<float> get_position() { return m_position; }

    std::vector<float> get_rotation() { return m_rotation; }

private:

    int m_id;

    int m_life;

    int m_shield;

    std::vector<float> m_position;

    std::vector<float> m_rotation;

};

#endif //CPP18_ASTEROIDS_LIGHT_SHIP_HPP
