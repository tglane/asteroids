//
// Created by timog on 18.03.19.
//

#include "light_object.hpp"

void light_object::move()
{
    m_position[0] = m_direction[0] * m_speed + m_position[0];
    m_position[1] = m_direction[1] * m_speed + m_position[1];
    m_position[2] = m_direction[2] * m_speed + m_position[2];
}
