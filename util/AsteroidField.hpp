/*
 *  AsteroidField.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef STARFIELD_HPP_
#define STARFIELD_HPP_


#include "rendering/Asteroid.hpp"
#include "math/Vector.hpp"

#include <list>
#include <memory>
#include <algorithm>
#include <cmath>

namespace asteroids
{

/**
 * @brief   Representatio of an asteroid field
 */
class AsteroidField 
{
public:
	using Ptr = std::shared_ptr<AsteroidField>;

	/**
	 * @brief   Creates an asteroid field with n asteroids in it
	 */
	AsteroidField(int n);

	/**
	 * @brief   Dtor.
	 */
	virtual ~AsteroidField();


	void getAsteroids(std::list<PhysicalObject::Ptr>& asteroids);

	void addAsteroid(PhysicalObject::Ptr asteroid);

private:

   /// The asteroids in the field
   std::list<PhysicalObject::Ptr> m_asteroids;
};

}

#endif

