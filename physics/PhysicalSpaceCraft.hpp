/*
 *  SpaceCraft.hpp
 *
 *  Created on: Nov. 24 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __PhysicalSpaceCraft_HPP__
#define __PhysicalSpaceCraft_HPP__

#include "Hittable.hpp"
#include "math/Vector.hpp"

#include <memory>

namespace asteroids
{
/**
 * @brief A class to render a cicle to the screen
 * 
 */
class PhysicalSpaceCraft : public Hittable
{
public:
    /**
     * @brief Construct a new PhysicalSpaceCraft object
     * 
     * @param filename the filename of the Trianglemesh
     * @param position the start position
     * @param movespeed movespeed for the key handling
     * @param rotatespeed rotatespeed for the key handling
     */
    PhysicalSpaceCraft(const Vector3f& position, float movespeed, float rotatespeed);

    /// dtor
    virtual ~PhysicalSpaceCraft();

    /**
     * @brief moves and rotates the mesh
     * 
     * @param keyStates the SDL Keyinput
     */
   // void handleKeyInput(const Uint8* keyStates);


private:
    TriangleMesh::Ptr m_mesh;

};

}

#endif
