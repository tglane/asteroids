/*
 *  PLYIO.hpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __PLYIO_HPP__
#define __PLYIO_HPP__

#include <string>

namespace asteroids
{
    

/**
 * @brief Loads a ply-File and creates vertex and index buffer arrays.
 *
 * @param file The name of the ply-File.
 * @param vertexBuffer Pointer to the created vertex buffer object or NULL
 *                     if the given file could not be parsed
 * @param indexBuffer  Pointer to the created vertex buffer object or NULL
 *                     if the given file could not be parsed
 * @param vertexCount  The number of loaded vertices or 0.
 * @param indexCount   The number of triangles in the ply model or 0.
 */
 void LoadPLY(const std::string& filename, 
              float*& vertexBuffer, 
              int*& indexbuffer,
              int& vertexCount,
              int& indexCount);

}

#endif
