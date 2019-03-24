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

#ifndef __SpaceCraft_HPP__
#define __SpaceCraft_HPP__

#include "Renderable.hpp"
#include "TriangleMesh.hpp"
#include "physics/Hittable.hpp"
#include "math/Vector.hpp"

#include <memory>

namespace asteroids
{
/**
 * @brief A class to render a cicle to the screen
 * 
 */
class SpaceCraft : public Renderable, public Hittable
{
public:
    using Ptr = std::shared_ptr<SpaceCraft>;

    SpaceCraft(const std::string& filename, const Vector3f& position, float movespeed, float rotatespeed);

    /**
     * @brief moves and rotates the mesh
     * 
     * @param keyStates the SDL Keyinput
     */

    /// renders the SpaceCraft
    virtual void render() override;

    /**
     * @brief check if the SpaceCraft has a valid mesh
     * 
     * @return true if mesh is valid
     * @return false if mesh is not valid
     */
    bool hasMesh() const;

    void fixArrow();

private:
    TriangleMesh::Ptr m_mesh;

};

}

#endif
