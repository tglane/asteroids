//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_SHIP_HPP
#define CPP18_ASTEROIDS_LIGHT_SHIP_HPP

#include <vector>

#include "light_object.hpp"

class light_ship : public light_object {

public:

    light_ship(int id) : light_object(id) {}

    int get_life() { return m_life; }

    int get_shield() { return m_shield; }


private:

    int m_life;

    int m_shield;

};

#endif //CPP18_ASTEROIDS_LIGHT_SHIP_HPP
