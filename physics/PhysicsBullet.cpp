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

PhysicsBullet::PhysicsBullet(const Vector3f& fighter_position, const Vector3f fighter_axis)
	: m_alive(true)
{
    // the bullet will move on this axis, it has to be reversed for the direction to be right    
    this->m_fighterAxis = fighter_axis;
	this->m_iterations = 0;
	this->m_speed = 10;
    this->m_radius = 10;

    // same position as the fighter
	m_position = fighter_position;
}

void PhysicsBullet::run()
{
	m_position = m_fighterAxis  * m_speed + m_position;
	//cout << m_position << endl;
	m_iterations++;
	
	if(m_iterations > m_lifetime)
	{
		m_alive = false;
	}
	
}

int PhysicsBullet::radius()
{
	return m_radius;
}


} // namespace asreroids

