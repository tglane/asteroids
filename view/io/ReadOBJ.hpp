/*
 * ReadOBJ.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef ReadOBJ_HPP_
#define ReadOBJ_HPP_

#include "MeshReader.hpp"
#include "rendering/TexturedMesh.hpp"

#include <map>
#include <climits>
#include <iostream>
#include <fstream>
#include <string.h>
#include <locale.h>
#include <sstream>

using namespace std;

namespace asteroids
{

/**
 * @brief   A basic implementation to read models from Wavefront
 *          .obj files 
 *          
 */
class ReadOBJ : public MeshReader
{
public:

	/**
	 * @brief
	 **/
	ReadOBJ(const std::string& filename) : m_filename(filename) {}

	virtual ~ReadOBJ() { };

    auto getMesh() -> TriangleMesh::Ptr;

    void getMesh(TexturedMesh& mesh);
	

private:

	void parseMtlFile(map<string, int>& matNames,
			vector<Material::Ptr>& materials,
			vector<Texture::Ptr>& textures,
			string mtlname);

    std::string m_filename;
	
};

} // namespace asteroids

#endif /* ReadOBJ_H_ */
