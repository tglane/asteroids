/*
 *  Normal.hpp
 *
 *  @date 07.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef NORMAL_HPP_
#define NORMAL_HPP_


#include "Vector.hpp"

namespace asteroids {

/**
 * @brief	A normal implementation. Basically a vertex
 * 			with normalization functionality.
 */
template<typename CoordType>
class Normal : public Vector<CoordType>
{

public:

	/**
	 * @brief	Default constructor. All elements are set
	 * 			to zero.
	 */
	Normal() : Vector<CoordType>() {};

	/**
	 * @brief	Constructs a new normal from the given
	 * 			components. Applies normalization.
	 */
	Normal(CoordType x, CoordType y, CoordType z)
	: Vector<CoordType>(x, y, z)
	  {
		this->normalize();
	  }

	/**
	 * @brief	Copy constructor for vertices. Applies
	 * 			normalization.
	 */
	template<typename T>
	Normal(const Vector<T> &other) : Vector<T>(other)
	{
		this->normalize();
	}

	/**
	 * @brief	Constructs from another normal. Implemented
	 * 			to avoid unnecessary normalizations.
	 */
	template<typename T>
	Normal(const Normal &other)
	{
		this[0] = other[0];
		this[1] = other[1];
		this[2] = other[2];
	}

	virtual ~Normal() = default;

	CoordType operator[](int index) const
	{
		return this->m[index];
	}


	Normal<CoordType> operator+(const Normal &n) const
	{
	    return Normal<CoordType>(this->m[0] + n[0], this->m[1] + n[1], this->m[2] + n[2]);
	}

	virtual Normal<CoordType> operator-(const Normal &n) const
	{
		return Normal<CoordType>(this->m[0] - n[0], this->m[1] - n[1], this->m[2] - n[2]);
	}

	void operator+=(const Normal &n)
	{
		*this = *this + n;
	}

	void operator-=(const Normal &n)
	{
		*this = *this + n;

	}
};


} // namespace asteroids


#endif /* NORMAL_HPP_ */
