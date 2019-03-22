/*
 *  PhysicalSpaceCraft.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "PhysicalSpaceCraft.hpp"
#include "io/TriangleMeshFactory.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

    PhysicalSpaceCraft::PhysicalSpaceCraft(const Vector3f& position, float movespeed, float rotatespeed, int id = 0)
{
    setId(id);
    m_speed = movespeed;
    m_rotationSpeed = rotatespeed;
}



PhysicalSpaceCraft::~PhysicalSpaceCraft()
{
 
}

} // namespace asteroids
