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

AsteroidField::AsteroidField(int quantity, const std::string& filename, float rangemax, float sizemin, float sizemax)
{
	TexturedMesh::Ptr mesh = std::static_pointer_cast<TexturedMesh>(TriangleMeshFactory::instance().getMesh(filename));
 	// Generate asteroids
	for(int i = 0; i < quantity; i++)
	{
	  Asteroid::Ptr p = make_shared<Asteroid>(Asteroid(
		  mesh,  Randomizer::instance()->getRandomVertex(1.0),           // Orientation
                Randomizer::instance()->getRandomVertex(1000),          // Position
                Randomizer::instance()->getRandomNumber(0, 100),        // Mass
                Randomizer::instance()->getRandomNumber(0, 1.57079633), // Rotation
                Randomizer::instance()->getRandomNumber(0, 0.05),          // Speed
                Randomizer::instance()->getRandomNumber(0, 0),          // Acceleration
                Randomizer::instance()->getRandomNumber(20, 100)         // Radius)
	  ));
	  m_asteroids.push_back(p);
	}
}

void AsteroidField::getAsteroids(std::list<Asteroid::Ptr>& out)
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
