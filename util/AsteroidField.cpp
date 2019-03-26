/*
 *  AsteoridField.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "AsteroidField.hpp"
#include "math/Randomizer.hpp"
#include "rendering/Asteroid.hpp"

namespace asteroids
{

AsteroidField::AsteroidField(int quantity)
{
	//TexturedMesh::Ptr mesh = std::static_pointer_cast<TexturedMesh>(TriangleMeshFactory::instance().getMesh(filename));
 	// Generate asteroids
    //addAsteroid(Asteroid::Ptr(new Asteroid(mesh, Vector3f(), Vector3f(1000, 0, 0), 0, 0, 0, 0, 100, 1)));
    // addAsteroid(Asteroid::Ptr(new Asteroid(mesh, Vector3f(), Vector3f(-1000, 0, 0), 0, 0, 0, 0, 100, 2)));
    // addAsteroid(Asteroid::Ptr(new Asteroid(mesh, Vector3f(), Vector3f(0, 1000, 0), 0, 0, 0, 0, 100, 3)));
    // addAsteroid(Asteroid::Ptr(new Asteroid(mesh, Vector3f(), Vector3f(0, -1000, 0), 0, 0, 0, 0, 100, 4)));
    int next_id = 0;
	for(int i = 0; i < quantity; i++)
	{
	  PhysicalObject::Ptr p = make_shared<PhysicalObject>(PhysicalObject(
                Randomizer::instance()->getRandomVertex(1.0),           // Orientation
                Randomizer::instance()->getRandomVertex(1000),          // Position
                Randomizer::instance()->getRandomNumber(0, 100),        // Mass
                0,
                Randomizer::instance()->getRandomNumber(0, 0.05),          // Speed
                Randomizer::instance()->getRandomNumber(0, 0),          // Acceleration
                Randomizer::instance()->getRandomNumber(20, 100),         // Radius
                next_id++
                                                        ));
	  m_asteroids.push_back(p);
	}
}

void AsteroidField::getAsteroids(std::list<PhysicalObject::Ptr>& out)
{
  //  std::copy(m_asteroids.begin(), m_asteroids.end(), out.begin());
  for(auto it = m_asteroids.begin(); it != m_asteroids.end(); it++)
  {
	  out.push_back( *it);
  }
}


AsteroidField::~AsteroidField()
{
	//asteroids.for_each(deleteAsteroid);
	
}

} // namespace asteroids
