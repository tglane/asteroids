//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_ASTEROID_HPP
#define CPP18_ASTEROIDS_LIGHT_ASTEROID_HPP


class light_asteroid {

public:

    light_asteroid(int id, float size, std::vector<float> position, std::vector<float> rotation)
            : m_id(id), m_size(size), m_position(position)

    int get_id() { return m_id; }

    float get_size() { return m_size; }

    std::vector<float> get_position() { return m_position; }

private:

    int m_id;

    float m_size;

    std::vector<float> m_position;

};


#endif //CPP18_ASTEROIDS_LIGHT_ASTEROID_HPP
