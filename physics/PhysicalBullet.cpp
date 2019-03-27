/*
 *  Bullet.cpp
 *
 *  @date 19.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */



#include "PhysicalBullet.hpp"

#include "util/gl_includes.h"

namespace asteroids
{

PhysicalBullet::PhysicalBullet(const Vector3f &fighter_position, const Vector3f fighter_axis, int shooter_id, int id)
		: PhysicalObject(id) {
	// the bullet will move on this axis, it has to be reversed for the direction to be right
	this->m_fighterAxis = fighter_axis;
	this->m_iterations = 0;
	this->m_speed = 50;
    this->m_radius = 10;
    this->m_shooter_id = shooter_id;

	// same position as the fighter
	m_position = fighter_position;
}

void PhysicalBullet::run(int elapsed_time) {
	m_position = m_fighterAxis * (m_speed * 60 * elapsed_time / 1000.0) + m_position;
	m_iterations++;

	if (m_iterations > m_lifetime) {
		m_alive = false;
	}
}

} // namespace asreroids

