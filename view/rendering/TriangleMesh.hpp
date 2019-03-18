/*
 *  TriangleMesh.hpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __TriangleMesh_HPP__
#define __TriangleMesh_HPP__

#include <string>
#include <memory>

#include "util/SharedArray.hpp"
#include "Renderable.hpp"

namespace asteroids
{
   
/**
 * @brief A struct to represent a simple 3D TriangleMesh 
 * 
 */
class TriangleMesh : public Renderable
{
public:

    using Ptr = std::shared_ptr<TriangleMesh>;
    using FloatArray = shared_array<float>;
    using IntArray = shared_array<int>;

    TriangleMesh();

    /**
     * @brief Construct a new TriangleMesh object from another instance
     * 
     * @param other         Instance to clone
     */
    TriangleMesh(const TriangleMesh& other);

    /**
	 * @brief   Contructs a triangle mesh from given buffers
	 * @param indexBuffer indexbuffer
	 * @param vertexBuffer vertexbuffer
	 * @param numFaces number of faces
	 * @param numVertices number of vertices
	 */
	TriangleMesh(IntArray indexBuffer, FloatArray vertexBuffer, FloatArray normals, int numFaces, int numVertices);


    /**
     * @brief Prints general information (number of vertices and faces) 
     *        to stdout.
     * 
     */
    void printTriangleMeshInformation();

    /**
     * @brief Prints the contends of the internal buffers to stdout.
     * 
     */
    void printBuffers();

    /**
     * @brief Renders the triangle mesh
     * 
     */
    virtual void render() override;

	/**
	 * @brief   Sets the normal buffer of the mesh
	 *
	 * @param normals The new normal buffer
	 */
	void setNormalBuffer(FloatArray normals) { m_normalBuffer = normals;}

    /**
     * @brief   Sets the vertex buffer of the mesh
     *
     * @param vertices  The new vertex buffer
     * @param n         The number of vertices in the mesh
     */
	void setVertexBuffer(FloatArray vertices, int n)
	{
	    m_vertexBuffer = vertices;
	    m_numVertices = n;
	}

    /**
     * @brief   Sets the index buffer of the mesh
     *
     * @param faces     The new index buffer
     * @param n         The number of faces in the mesh
     */

	void setIndexBuffer(IntArray faces, int n)
	{
	    m_indexBuffer = faces;
	    m_numFaces = n;
	}

    /**
     * @brief Destroys the TriangleMesh object
     * 
     */
    virtual ~TriangleMesh() = default;

    	/**
	 * @brief   Scales the mesh by the given factor
	 */
	void scale(float s);

protected:

    /// Number of vertices
    int         m_numVertices;

    /// Number of faces
    int         m_numFaces;

    /// Vertexbuffer
    FloatArray  m_vertexBuffer;

    /// Normalbuffer
    FloatArray  m_normalBuffer;

    /// Indexbuffer
    IntArray    m_indexBuffer;

    
};

} // namespace asteroids

#endif
