//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
#define CPP18_ASTEROIDS_LIGHT_OBJECT_HPP

#include <vector>

class light_object {

public:

    void move();

    int get_id() { return m_id; }

    std::vector<float> get_direction() { return m_direction; }

    void set_direction(float x, float y, float z) { m_direction(x, y, z); }

protected:

    int m_id();

    int m_speed;

    std::vector<float> m_position;

    std::vector<float> m_direction;

};


#endif //CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
