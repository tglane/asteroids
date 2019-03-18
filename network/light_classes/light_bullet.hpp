//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_BULLET_HPP
#define CPP18_ASTEROIDS_LIGHT_BULLET_HPP


class light_bullet {

public:

    light_bullet(int id, std::vector<float> position)
            : m_id(id), m_position(position)

    int get_id() { return m_id; }

    std::vector<float> get_position() { return m_position; }

private:

    int m_id;

    std::vector<float> m_position;

};


#endif //CPP18_ASTEROIDS_LIGHT_BULLET_HPP
