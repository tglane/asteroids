/**
 *  @file Quaternion.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */



#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

#include "Vector.hpp"

#include <iostream>
#include <cmath>

namespace asteroids
{

/**
 * @brief   Quaternion representation for OpenGL. Based on: 
 *          http://gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
 *
 */
class Quaternion {

public:

	/**
	 * @brief   Construcs a default quaternion object
	 */
	Quaternion();

	/**
	 * @brief   Destructor
	 */
	~Quaternion() = default;

	/**
	 * @brief   Construcs (with fromAxis()) a quaternion with a given Vector3f and an angle
	 * @param vec Vector3f
	 * @param angle angle
	 */
	Quaternion(const Vector3f& vec, float angle);

	/**
	 * @brief   Constructs a quaternion with three given values and an angle
	 * @param x x-value
	 * @param y y-value
	 * @param z z-value
	 * @param w angle
	 */
	Quaternion(float x, float y, float z, float w);

	/**
	 * @brief   Constructs a quaternion with a given float-pointer and an angle
	 * @param vec Vector3f(pointer)
	 * @param w angle
	 */
	Quaternion(float* vec, float w);
  
	/**
	 * @brief   Calculates a quaternion with a given Vector3f and an angle
	 * @param axis Vector3f
	 * @param angle angle
	 */
	void fromAxis(const Vector3f& axis, float angle);
 
	/**
	 * @brief   Conjugates a quaternion
	 * @return quaternion (conjugate)
	 */
	Quaternion getConjugate() const;

	/**
	 * @brief   Defines the quaternion-quaternion multiplication
	 * @param rq quaternion
	 * @return quaternion
	 */
	Quaternion operator* (const Quaternion& rq) const;

	/**
	 * @brief   Defines the Vector3f-quaternion multiplication
	 * @param vec Vector3f
	 * @return Vector3f
	 */
	Vector3f operator* (const Vector3f& vec) const;
  
private:

	/**
	 * @brief   Value of angle, x, y and z
	 */
	float w, x, y, z;
};
    
} // namespace asteroids

#endif

