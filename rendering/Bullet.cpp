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


namespace asteroids
{

Bullet::Bullet(const Vector3f& fighter_position, const Vector3f fighter_axis)
	: m_alive(true),  m_sphere(Vector3f(0, 0, 0), 10)
{
    // the bullet will move on this axis, it has to be reversed for the direction to be right    
    this->m_fighterAxis = fighter_axis;
	this->m_iterations = 0;
	this->m_speed = 10;

    // same position as the fighter
	m_position = fighter_position;
}

void Bullet::run()
{
	m_position = m_fighterAxis  * m_speed + m_position;
	//cout << m_position << endl;
	m_iterations++;
	
	if(m_iterations > m_lifetime)
	{
		m_alive = false;
	}
	
}

int Bullet::radius()
{
	return m_sphere.radius();
}

void Bullet::render()
{
    // Compute transformation matrix
	computeMatrix();
    // Push old transformation of the OpenGL matrix stack and
	// start rendering the bullet in according to the
	// internal transformation matrix
	glPushMatrix();
	glMultMatrixf(m_transformation.getData());
	//cout << m_transformation << endl;
	m_sphere.render();

	// Pop transformation matrix of this object
	// to restore the previous state of the OpenGL
	// matrix stack
	glPopMatrix();
}

} // namespace asreroids

