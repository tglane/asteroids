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

void Bullet::render() {
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


}
