/**
 *  @file Quaternion.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Quaternion.hpp"

namespace asteroids
{

Quaternion::Quaternion()
{
	// Default Quaternion
	x = 1.0;
	y = 0.0;
	z = 0.0;
	w = 0.0; 
}

Quaternion::Quaternion(const Vector3f& vec, float angle)
{
	// Calculate the quaternion
	fromAxis(vec, angle);
}

Quaternion::Quaternion(float _x, float _y, float _z, float _angle)
{
	// Set the values
	x = _x;
	y = _y;
	z = _z;
	w = _angle;
}

Quaternion::Quaternion(float* vec, float _w)
{
	// Set the values
	x = vec[0];
	y = vec[1];
	z = vec[2];
	w = _w;
}

void Quaternion::fromAxis(const Vector3f& axis, float angle)
{
	float sinAngle;
	angle *= 0.5f;

	// Create a copy of the given Vector3f and normalize the new Vector3f
	Vector3f vn(axis[0], axis[1], axis[2]);
	vn.normalize();
 
	// Calculate the sinus of the given angle
	sinAngle = sin(angle);
 
	// Get the quaternion
	x = (vn[0] * sinAngle);
	y = (vn[1] * sinAngle);
	z = (vn[2] * sinAngle);
	w = cos(angle);
}

Quaternion Quaternion::getConjugate() const
{
	// Conjugate the given quaternion
	return Quaternion(-x, -y, -z, w);
}


Quaternion Quaternion::operator* (const Quaternion& rq) const
{
	// Calculate the new quaternion
	return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
				w * rq.y + y * rq.w + z * rq.x - x * rq.z,
				w * rq.z + z * rq.w + x * rq.y - y * rq.x,
				w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

Vector3f Quaternion::operator* (const Vector3f& vec) const
{
	// Copy the Vector3f and normalize the new Vector3f
	Vector3f vn(vec);
	vn.normalize();

	// Fill the first quaternion and...
	Quaternion vecQuat, resQuat;
	vecQuat.x = vn[0];
	vecQuat.y = vn[1];
	vecQuat.z = vn[2];
	vecQuat.w = 0.0f;

	// calculate the new quaternion
	resQuat = vecQuat * getConjugate();
	resQuat = *this * resQuat;
  	return (Vector3f(resQuat.x, resQuat.y, resQuat.z));
}
    
}
