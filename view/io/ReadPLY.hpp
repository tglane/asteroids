/*
 * ReadPLY.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __PLY_IO_H__
#define __PLY_IO_H__

#include "PLYProperty.hpp"
#include "PLYElement.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#include <cassert>
#include "MeshReader.hpp"

using std::ofstream;
using std::ifstream;
using std::vector;
using std::cout;
using std::endl;

#define BUFFER_SIZE 1024
#define OUT_BUFFER_SIZE 20000

namespace asteroids
{

/**
 * @brief A class for input and output to ply files.
 */
class ReadPLY : public MeshReader
{

public:

	/**
	 * @brief Ctor.
	 */
	ReadPLY(const string& filename);

	/**
	 * @brief Adds the given element to the file
	 *
	 * @param element 		A ply element description
	 */
	void addElement(PLYElement* e);

	/**
	 * @brief Sets the vertex array (for meshes)
	 *
	 * @param array			A vertex array
	 * @param n				The number of vertices in the array
	 *
	 * This version uses an interlaced array. Hence the number of floats
	 * in the array is 3 * \ref{n}.
	 */
	void setVertexArray(float* array, size_t n);

	/**
	 * @brief Sets the normal array (for meshes)
	 *
	 * @param array			A set of normal coordinated
	 * @param n				The number of normals in the array
	 *
     * This version uses an interlaced array. Hence the number of floats
	 * in the array is 3 * \ref{n}.
	 */
	void setNormalArray(float* array, size_t n);

	/**
	 * @brief Sets the color array (for vertex colors)
	 *
	 * @param array			An array containing color information
	 * @param n				The number of elements in the array
	 */
	void setColorArray(float* array, size_t n);

	/**
	 * @brief Sets the index buffer
	 *
	 * @param array 		A index buffer
	 * @param n				The number of faces encoded in the buffer
	 *
	 * The number of face in the buffer is \ref{n} / 3 since each face consists
	 * of three vertices that are referenced in the buffer.
	 */
	void setIndexArray(int* array, size_t n);

	/**
	 * @brief Save the currently present information to the given file
	 *
	 * @param filename		The output file
	 * @param binary		If, the data is writen in binary format (default). Set
	 * 						this param to false to create an ASCII ply file
	 */
	void save(string filename, bool binary = true);

	/**
	 * @brief Reads all supported information from the given file
	 * @param filename		A ply file
	 */
	void read(string filename);

	/**
	 * @brief Dtor.
	 */
	virtual ~ReadPLY();

	/**
	 * @brief Returns the interlaced vertex array (or a null pointer if
	 * 		  not set).
	 * @param n				Contains the number of Vertices in the array
	 * @return				A pointer to vertex data
	 */
	float* getVertexArray(size_t &n);

	/**
	 * @brief Returns the interlaced normal array (or a null pointer if
	 * 		  not set).
	 * @param n				Contains the number of Vertices in the array
	 * @return				A pointer to normal data
	 */
	float* getNormalArray(size_t &n);

	/**
	 * @brief Returns the interlaced color array (or a null pointer if
	 * 		  not set).
	 * @param n				Contains the number of Vertices in the array
	 * @return				A pointer to color data
	 */
	float* getColorArray(size_t &);

	/**
	 * @brief Returns an index accessible representation (2D array) of
	 * 		  the vertex data.
	 *
	 * @param n				Contains the number of vertices
	 * @return				A pointer to 2D vertex data.
	 *
	 * Using this method, the preferred interlaced representation is
	 * converted into a 2D array. Be careful with large data sets since
	 * the information is duplicated.
	 */
	float** getIndexedVertexArray(size_t &n);


	/**
	 * @brief Returns an index accessible representation (2D array) of
	 * 		  the vertex data.
	 *
	 * @param n				Contains the number of vertices
	 * @return				A pointer to 2D vertex data.
	 *
	 * Using this method, the preferred interlaced representation is
	 * converted into a 2D array. Be careful with large data sets since
	 * the information is duplicated.
	 */
	float** getIndexedNormalArray(size_t &n);

	/**
	 * @brief Adds indexed vertex data.
	 *
	 * @param arr			Indexed vertex data
	 * @param size			The number of vertices in the provided 2D array
	 *
	 * The provided data is converted. Beware of memory overhead.
	 */
	void setIndexedVertexArray(float** arr, size_t size);

	/**
	 * @brief Adds indexed vertex data.
	 *
	 * @param arr			Indexed vertex data
	 * @param size			The number of vertices in the provided 2D array
	 *
	 * The provided data is converted. Beware of memory overhead.
	 */
	void setIndexedNormalArray(float** arr, size_t size);

	/**
	 * @brief Returns the index array of a mesh
	 * @param n 			The number of faces in the mesh
	 * @return				A pointer to the index data
	 */
	 int* getIndexArray(size_t &n);

	/**
	 * @brief Returns true if the current element contains the provided
	 * 		  element
	 * @param e				A ply element description object
	 */
	bool containsElement(PLYElement& e);

	/**
	 * @brief Returns true if the current element lists contains an
	 *        element with the given name.
	 */
	bool containsElement(string elementName);

	/**
	 * @brief Checks if \ref{e} has property \ref{p}
	 */
	bool hasProperty(PLYElement& e, Property& p);

	/**
	 * @brief Prints all elements and properties to stdout.
	 */
	void printElementsInHeader();

	/**
	 * @brief Returns a triangle mesh representation of the loaded elements
	 */
	auto getMesh() -> TriangleMesh::Ptr;

private:

	float** interlacedBufferToIndexedBuffer(float* src, size_t n);
	float*	indexedBufferToInterlacedBuffer(float** src, size_t n);

	void writeHeader(ofstream& str);
	void writeElements(ofstream &str);
	void writeFacesBinary(ofstream &str, PLYElement* e);
	void writeFacesASCII(ofstream &str, PLYElement* e);
	void writeVerticesBinary(ofstream &str, PLYElement* e);
	void writeVerticesASCII(ofstream &str, PLYElement* e);
	void writeNormalsBinary(ofstream &out, PLYElement* e);
	void writeNormalsASCII(ofstream &out, PLYElement* e);

	void readVerticesBinary(ifstream &in, PLYElement* descr);
	void readFacesBinary(ifstream &in, PLYElement* descr);
	void readNormalsBinary(ifstream &in, PLYElement* descr);

	void readVerticesASCII(ifstream &in, PLYElement* descr);
	void readFacesASCII(ifstream &in, PLYElement* descr);
	void readNormalsASCII(ifstream &in, PLYElement* descr);

	void readHeader(ifstream& str);

	char* putElementInBuffer(char* buffer, string s,  float value);

	bool isSupported(string element_name);
	bool parseHeaderLine(const char* line);

	void loadElements(ifstream& in);

	void deleteBuffers();
	void allocVertexBuffers(PLYElement* dscr);

	void copyElementToVertexBuffer(ifstream &str, Property*, float* buffer, size_t position);

	template<typename T>
	void copyElementToVertexBuffer(char* src, float* buffer, size_t positon);

	float*					m_vertices;
	float*					m_normals;
	float*					m_colors;
	int*			m_indices;

	size_t					m_numberOfNormals;
	size_t					m_numberOfVertices;
	size_t					m_numberOfFaces;

	bool 					m_binary;

	vector<PLYElement*> 	m_elements;

};

}

#endif
