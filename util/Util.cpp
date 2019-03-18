/*
 *  Util.cpp
 *  Created on: Dec 08, 2017
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2017 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Util.hpp"

#include <iostream>
#include <fstream>

namespace asteroids
{

std::string GetExtensionFromFileName(std::string filename)
{
    // Get path from given .lvl file
    size_t position = filename.find_last_of(".");
    if(position != std::string::npos)
    {
        return filename.substr(position + 1);
    }
    return "";
}

std::string GetPathFromFileName(std::string filename)
{
    size_t position = filename.find_last_of("/\\");
    return filename.substr(0, position + 1);
}

} // namespace asteroids
