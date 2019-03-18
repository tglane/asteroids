/*
 * ReadOBJ.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include <boost/filesystem.hpp>
#include "boost/tuple/tuple.hpp"

#include "ReadOBJ.hpp"
#include "TextureFactory.hpp"

#include "rendering/Materials.hpp"
#include "rendering/Texture.hpp"


namespace asteroids
{

void tokenize(const std::string& str,
                    std::vector<string>& tokens,
                    const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void ReadOBJ::parseMtlFile(
		map<string, int>& matNames,
		vector<Material::Ptr>& materials,
		vector<Texture::Ptr>& textures,
		string mtlname)
{
	cout << "Parsing " << mtlname << endl;

	// Get path object
	boost::filesystem::path p(mtlname);
	p = p.remove_filename();

	ifstream in(mtlname.c_str());
	if(in.good())
	{
		char buffer[1024];
		Material::Ptr m = 0;
		int matIndex = 0;
		while(in.good())
		{
			in.getline(buffer, 1024);

			// Skip comments
			if(buffer[0] == '#') continue;

			stringstream ss(buffer);
			string keyword;
			ss >> keyword;

			if(keyword == "newmtl")
			{
				string matName;
				ss >> matName;
				map<string, int>::iterator it = matNames.find(matName);
				if(it == matNames.end())
				{
					Material::Ptr m(new Material);
					m->m_ambient.r = 128;
					m->m_ambient.g = 128;
					m->m_ambient.b = 128;
					materials.push_back(m);
					matNames[matName] = matIndex;
					matIndex++;
				}
				else
				{
					//m = materials[matNames[matName]];
					cout << "ObjIO::parseMtlFile(): Warning: Duplicate material: " << matName << endl;
				}
			}
			else if(keyword == "Ka")
			{
				float r, g, b;
				ss >> r >> g >> b;
				m->m_ambient.r = (unsigned char)(r * 255);
				m->m_ambient.g = (unsigned char)(g * 255);
				m->m_ambient.b = (unsigned char)(b * 255);
			}
			else if(keyword == "map_Kd")
			{
				string texname;
				ss >> texname;

				// Add full path to texture file name
				boost::filesystem::path tex_file = p / texname;

				Texture::Ptr texture = TextureFactory::instance().getTexture(tex_file.string());
				m->m_texture = texture;
			}
			else
			{
				continue;
			}
		}
	}
	else
	{
		cout << "ObjIO::parseMtlFile(): Error opening '" << mtlname << "'." << endl;
	}
}

void ReadOBJ::getMesh(TexturedMesh& texMesh)
{
    // Get path from filename
    boost::filesystem::path p(m_filename);

	ifstream in(m_filename.c_str());

	vector<float> 		vertices;
	vector<float> 		normals;
	vector<float> 		texcoords;
	//vector<uint>		faceMaterials;
	vector<uint>  		faces;
	vector<Material::Ptr> 	materials;
	vector<Texture::Ptr>	textures;

	map<string, int> matNames;
    map<int, MaterialFaceList::Ptr> matGroups;
    MaterialFaceList::Ptr currentMatList = 0;

	int currentMat = 0;

	if(in.good())
	{
		char buffer[1024];
		while(in.good())
		{
			in.getline(buffer, 1024);

			// Skip comments
			if(buffer[0] == '#') continue;

			stringstream ss(buffer);
			string keyword;
			ss >> keyword;
			float x, y, z;
			if(keyword == "v")
			{
				ss >> x >> y >> z;
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
			else if(keyword == "vt")
			{
				ss >> x >> y >> z;
				texcoords.push_back(x);
				texcoords.push_back(1 - y);
				texcoords.push_back(z);
			}
			else if(keyword == "vn")
			{
				ss >> x >> y >> z;
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);
			}
			else if(keyword == "f")
			{
				vector<string> tokens;
				tokenize(buffer, tokens);

				if(tokens.size() < 4)
					continue;

				vector<string> tokens2;
				tokenize(tokens.at(1),tokens2,"/");
				int a = atoi(tokens2.at(0).c_str());
				tokens2.clear();

				tokenize(tokens.at(2),tokens2,"/");
				int b = atoi(tokens2.at(0).c_str());
				tokens2.clear();

				tokenize(tokens.at(3),tokens2,"/");
				int c = atoi(tokens2.at(0).c_str());
				tokens2.clear();

				faces.push_back(a - 1);
				faces.push_back(b - 1);
				faces.push_back(c - 1);

				// Use current material
                if(currentMatList)
                {
                    currentMatList->m_faces.push_back(a - 1);
                    currentMatList->m_faces.push_back(b - 1);
                    currentMatList->m_faces.push_back(c - 1);
                }
				else
                {
                    cout << "ReadOBJ: Current material " << currentMat << " is not defined." << endl;
                }
			}
			else if(keyword == "usemtl")
			{
				string mtlname;
				ss >> mtlname;
				// Find name and set current material
				map<string, int>::iterator it = matNames.find(mtlname);
				if(it == matNames.end())
				{
					cout << "ObjIO:read(): Warning material '" << mtlname << "' is undefined." << endl;
				}
				else
				{
                    // Check if material face lists already exists
                    // and insert if necessary
                    map<int, MaterialFaceList::Ptr>::iterator matIt = matGroups.find(it->second);
                    if(matIt == matGroups.end())
                    {
                        MaterialFaceList::Ptr l(new MaterialFaceList);
                        l->m_matIndex = it->second;
                        currentMat = it->second;
                        matGroups[it->second] = l;
                        currentMatList = l;
                    }
                    else
                    {
                        currentMatList = matIt->second;
                        currentMat = it->second;
                    }
                    
				}
			}
			else if(keyword == "mtllib")
			{
			    // Get current path
			    p = p.remove_filename();

			    // Append .mtl file name
				string mtlfile;
				ss >> mtlfile;
				p = p / mtlfile;

				// Get path as string and parse mtl
				string mtl_path = p.string();
				parseMtlFile(matNames, materials, textures, mtl_path);
			}
		}

	}
	else
	{
		cout << "ReadOBJ::getMesh(): Unable to open file'" << m_filename << "'." << endl;
	}

    // Copy read vertices to mesh
    FloatArray vertex_buffer(new float[vertices.size()]);
    for(size_t i = 0; i < vertices.size(); i++)
    {
        vertex_buffer[i] = vertices[i];
    }
    texMesh.setVertexBuffer(vertex_buffer, (int)vertices.size() / 3);

    // Copy read normals to mesh
    FloatArray normal_buffer (new float[normals.size()]);
    for(size_t i = 0; i < normals.size(); i++)
    {
        normal_buffer[i] = normals[i];
    }
    texMesh.setNormalBuffer(normal_buffer);
  
    // Copy texture coordinates
    FloatArray texture_coord_buffer(new float[texcoords.size()]);
    for(size_t i = 0; i < texcoords.size(); i++)
    {
        texture_coord_buffer[i] = texcoords[i];
    }
    texMesh.setTextureBuffer(texture_coord_buffer);

    // Copy materials 
    for(auto i : materials)
    {
        texMesh.addMaterial(i);
    }

    for(map<int, MaterialFaceList::Ptr>::iterator it = matGroups.begin(); 
        it != matGroups.end(); it++)
    {
        texMesh.addMaterialFaceList(it->second);
    }
    

}

auto ReadOBJ::getMesh() -> TriangleMesh::Ptr
{
    TexturedMesh* mesh = new TexturedMesh;
	getMesh(*mesh);

	return TriangleMesh::Ptr(mesh);
}

} // Namespace lvr
