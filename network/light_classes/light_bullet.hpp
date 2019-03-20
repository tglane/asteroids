//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_BULLET_HPP
#define CPP18_ASTEROIDS_LIGHT_BULLET_HPP

#include <vector>

#include "light_object.hpp"

class light_bullet : public light_object {

public:

    light_bullet(int id): light_object(id) {};
};


#endif //CPP18_ASTEROIDS_LIGHT_BULLET_HPP
