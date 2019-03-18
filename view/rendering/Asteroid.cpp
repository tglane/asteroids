/*
 *  Asteroid.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Asteroid.hpp"
#include "math/Randomizer.hpp"

namespace asteroids
{

Asteroid::Asteroid(
    TexturedMesh::Ptr mesh, 
    Vector3f startDirection, 
    Vector3f startPosition,
    float mass,
    float rotation,
    float speed,
    float accell,
    float radius)
    : PhysicalObject(mesh, startDirection, startPosition, mass, rotation, speed, accell, radius)
{
    // cout << startDirection << endl;
    // cout << startPosition << endl;
    // cout << mass << " " << rotation << " " << speed << " " << accell << " " << radius << endl;

    //Randomize Heading
    rotate(PITCH_LEFT, Randomizer::instance()->getRandomNumber(0, 3.14159265));
    rotate(YAW_LEFT  , Randomizer::instance()->getRandomNumber(0, 3.14159265));
    rotate(ROLL_LEFT , Randomizer::instance()->getRandomNumber(0, 3.14159265));

    m_scale     = radius / MODEL_RADIUS;

    computeMatrix();
}

Asteroid& Asteroid::operator=( Asteroid& other )
{
	if(&other != this)
	{
		copy(other);
	}
	return *this;
}


Asteroid::Asteroid(const Asteroid& other)
{
	copy(other);
}

void Asteroid::copy(const Asteroid& other)
{
    m_position = other.m_position;
    TexturedMesh *mesh = static_cast<TexturedMesh *>(other.m_renderable.get());
    m_renderable = TexturedMesh::Ptr(new TexturedMesh(*mesh));
    m_radius = other.m_radius;
    m_rotation = other.m_rotation;
    m_xAxis = other.m_xAxis;
    m_yAxis = other.m_yAxis;
    m_zAxis = other.m_zAxis;
    m_scale = other.m_scale;
    m_speed = other.m_speed;
    m_direction = other.m_direction;
}

void Asteroid::render()
{
    // Compute transformation matrix
    computeMatrix();

    // Push matrix and set transformation and scaling
    glPushMatrix();
    glMultMatrixf(m_transformation.getData());

    glScalef(m_scale, m_scale, m_scale);
    if(m_renderable)
    {
        m_renderable->render();
    }
    glPopMatrix();
}

} // namespace asteroids
