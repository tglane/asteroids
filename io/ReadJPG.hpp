/*
 * ReadJPG.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef READJPG_HPP_
#define READJPG_HPP_

#include <iostream>
#include <fstream>

#include <jpeglib.h>
#include <jerror.h>

#include <string>
using std::string;

#include "BitmapReader.hpp"

namespace asteroids
{

class ReadJPG : public BitmapReader
{
public:

	/**
	 * @brief Constructor
	 */
    ReadJPG(string filename);

    /**
     * @brief Destruktor
     */
    ~ReadJPG();

};

}






#endif /* READJPG_HPP_ */
