/*
 * ReadTIFF.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef READTIFF_HPP_
#define READTIFF_HPP_

#include <string>
using std::string;

#include "BitmapReader.hpp"

namespace asteroids
{

class ReadTIFF : public BitmapReader
{
public:

	/**
	 * @brief Reads the given file
	 */
    ReadTIFF(string filename);
    virtual ~ReadTIFF();

};

}

#endif /* READTIFF_HPP_ */
