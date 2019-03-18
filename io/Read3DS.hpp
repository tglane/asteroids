/*
 * Read3DS.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#ifndef READ3DS_HPP_
#define READ3DS_HPP_

#include "rendering/TriangleMesh.hpp"
#include "rendering/TexturedMesh.hpp"
#include "rendering/Materials.hpp"

#include "TextureFactory.hpp"

#include <string>
#include <vector>

#include "MeshReader.hpp"
using std::vector;
using std::string;

namespace asteroids
{

/// Reads textures triangle mesh from a .3ds file
class Read3DS : public MeshReader
{
public:
    /// A 3ds file
    Read3DS(std::string filename);

    /// Retruns a triangle mesh representation of the 
    /// loaded mesh
    auto getMesh() -> TriangleMesh::Ptr;

    /// Loads the parsed data into the given mesh instance
    void getMesh(TexturedMesh::Ptr texMesh);

    /// Destructor
    virtual ~Read3DS();

private:

    void readSimpleMesh();
    void readTexturedMesh();

    /// Name of the parsed file
    string m_filename;
};

}
#endif /* READ3DS_HPP_ */
