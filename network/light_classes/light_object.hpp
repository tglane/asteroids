//
// Created by timog on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
#define CPP18_ASTEROIDS_LIGHT_OBJECT_HPP

#include <vector>
#include <math/Vector.hpp>

class light_object {

protected:

    uint32_t m_id;

    uint32_t m_speed;

    asteroids::Vector3f m_position;
    asteroids::Vector3f m_velocity;
    asteroids::Vector3f m_x_axis;
    asteroids::Vector3f m_y_axis;
    asteroids::Vector3f m_z_axis;


public:
    explicit light_object(uint32_t id);

    void move();

    int get_id() { return m_id; }

    void set_position(asteroids::Vector3f position) {m_position = position; }
    void set_velocity(asteroids::Vector3f velocity) {m_velocity = velocity; }
    void set_x_axis(asteroids::Vector3f x_axis) {m_x_axis = x_axis; }
    void set_y_axis(asteroids::Vector3f y_axis) {m_y_axis = y_axis; }
    void set_z_axis(asteroids::Vector3f z_axis) {m_z_axis = z_axis; }

    const asteroids::Vector3f& get_position() { return m_position; }
    const asteroids::Vector3f& get_velocity() { return m_velocity; }
    const asteroids::Vector3f& get_x_axis() { return m_x_axis; }
    const asteroids::Vector3f& get_y_axis() { return m_y_axis; }
    const asteroids::Vector3f& get_z_axis() { return m_z_axis; }



    std::vector<float> m_direction;

};


#endif //CPP18_ASTEROIDS_LIGHT_OBJECT_HPP
