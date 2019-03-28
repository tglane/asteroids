//
// Created by timog on 18.03.19.
//

#include "light_object.hpp"

light_object::light_object(uint32_t id) : m_id(id)
{
    m_x_axis = asteroids::Vector3f(1.0, 0.0, 0.0);
    m_y_axis = asteroids::Vector3f(0.0, 1.0, 0.0);
    m_z_axis = asteroids::Vector3f(0.0, 0.0, 1.0);
}

void light_object::move()
{
    m_position[0] = m_direction[0] * m_speed + m_position[0];
    m_position[1] = m_direction[1] * m_speed + m_position[1];
    m_position[2] = m_direction[2] * m_speed + m_position[2];
}
