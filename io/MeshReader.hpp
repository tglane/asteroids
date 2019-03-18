/**
 * MeshIO.h
 *
 *  @date 10.12.2011
 *  @author Thomas Wiemann
 */

#ifndef MESHIO_H_
#define MESHIO_H_

#include "rendering/TriangleMesh.hpp"
#include "util/SharedArray.hpp"

#include <memory>

namespace asteroids
{

/**
 * @brief Base class for Mesh in- and output
 */
class MeshReader
{
public:

    using FloatArray = shared_array<float>;
    using IntArray = shared_array<int>;
    using Ptr = std::shared_ptr<MeshReader>;

    MeshReader()
        : m_vertexBuffer(0),  // Initit all members with zero pointers
          m_normalBuffer(0),
          m_indexBuffer(0),
          m_numVertices(0),
          m_numFaces(0) {};


    /**
     * @brief Destructor. Nothing to do right now
     */
    virtual ~MeshReader() {};

    /**
     * @brief Returns a pointer to a triangle mesh instance
     */
    virtual auto getMesh() -> TriangleMesh::Ptr = 0;

protected:

    /// Vertex position information
    FloatArray  m_vertexBuffer;

    /// Vertex normal information
    FloatArray  m_normalBuffer;

    /// Triangle definitions
    IntArray    m_indexBuffer;

    /// Number of vertices in mesh
    int     m_numVertices;

    /// Number of faces in mesh
    int     m_numFaces;
};

}

#endif /* MESHIO_H_ */
