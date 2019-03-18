/*
 *  TriangleMeshFactory.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef TRIANGLEMESHFACTORY_H_
#define TRIANGLEMESHFACTORY_H_

#include <string>
#include "rendering/TriangleMesh.hpp"

using std::string;

namespace asteroids
{

/**
 * @brief   A factory class for loading triangle meshes. Implements to
 *          factory design pattern
 */
class TriangleMeshFactory
{
protected:

    /**
     * @brief Constructor
     */
    TriangleMeshFactory() {};

    /**
     * @brief Destructor
     */
    virtual ~TriangleMeshFactory() {};

public:

    /**
     * @brief Returns the mesh parsed from the file or a null pointer
     *        if an error occured
     */
    auto getMesh(const string &filename) const -> TriangleMesh::Ptr;

    /**
     * @brief Returns the instance pointer of the factory singleton
     */
    static TriangleMeshFactory& instance();

};

}
#endif /* TRIANGLEMESHFACTORY_H_ */
