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



#include "PhysicsBullet.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace asteroids
{

Bullet::Bullet(const Vector3f &fighter_position, const Vector3f fighter_axis, int shooter_id)
		: m_alive(true), m_sphere(Vector3f(0, 0, 0), 10), m_shooter_id(shooter_id) {
	// the bullet will move on this axis, it has to be reversed for the direction to be right
	this->m_fighterAxis = fighter_axis;
	this->m_iterations = 0;
	this->m_speed = 50;

	// same position as the fighter
	m_position = fighter_position;
}

void Bullet::run() {
	m_position = m_fighterAxis * m_speed + m_position;
	//cout << m_position << endl;
	m_iterations++;

	if (m_iterations > m_lifetime) {
		m_alive = false;
	}
}

}

int PhysicsBullet::radius()
{
	return m_radius;
}

int PhysicsBullet::get_shooter_id() const {
	return m_shooter_id;
}

} // namespace asreroids

