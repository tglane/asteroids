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



#include "Bullet.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace asteroids {

Bullet::Bullet(const Vector3f &fighter_position, const Vector3f fighter_axis, int shooter_id)
		: m_alive(true), m_sphere(Vector3f(0, 0, 0), 10), m_shooter_id(shooter_id) {
	// the bullet will move on this axis, it has to be reversed for the direction to be right
	this->m_fighterAxis = fighter_axis;
	this->m_iterations = 0;
	this->m_speed = 100;

	// same position as the fighter
	m_position = fighter_position;
}

void Bullet::run(int elapsed_time) {
	m_position = m_fighterAxis * (m_speed * 60 * elapsed_time / 1000.0) + m_position;
	m_iterations++;

	if (m_iterations > m_lifetime) {
		m_alive = false;
	}
}

int Bullet::radius() {
	return m_sphere.radius();
}

void Bullet::render() {
	// Compute transformation matrix
	computeMatrix();
	// Push old transformation of the OpenGL matrix stack and
	// start rendering the bullet in according to the
	// internal transformation matrix
	glPushMatrix();
	glMultMatrixf(m_transformation.getData());
	m_sphere.render();

	// Pop transformation matrix of this object
	// to restore the previous state of the OpenGL
	// matrix stack
	glPopMatrix();
}

int Bullet::get_shooter_id() const {
	return m_shooter_id;
}

}
