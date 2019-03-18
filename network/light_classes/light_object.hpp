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

protected:

    int m_id;

    int m_speed;

    std::vector<float> m_position;

};


#endif //CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
