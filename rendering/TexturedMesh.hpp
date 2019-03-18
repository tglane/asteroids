/*
 *  TextureMesh.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef TEXTUREDMESH_HPP_
#define TEXTUREDMESH_HPP_

#include <iostream>
#include <vector>
#include <memory>

using std::vector;
using std::cout;
using std::endl;

#include "Materials.hpp"
#include "TriangleMesh.hpp"
#include "Texture.hpp"

namespace asteroids
{

/**
 * @brief       Represents a texured TriangleMesh
 */
class TexturedMesh : public TriangleMesh
{
public:

    using Ptr = std::shared_ptr<TexturedMesh>;

    /**
     * @brief   Ctor
     */
    TexturedMesh();

    /**
     * @brief   Copy Ctor.
     */
    TexturedMesh(TexturedMesh &mesh);

    /**
     * @brief   Destructor
     */
    virtual ~TexturedMesh() = default;

    /**
     * @brief   Adds a material to the internal material list
     */
    void addMaterial(Material::Ptr mat)                 {m_materials.push_back(mat);}

    /**
     * @brief   Adds a new list with faces that share the same materials
     */
    void addMaterialFaceList(MaterialFaceList::Ptr l)   {m_matFaceLists.push_back(l);}

    /**
     * @brief   Sets the internal buffer with texture coordinates
     */
    void setTextureBuffer(FloatArray texCoords)         {m_textureCoords = texCoords;}

    /**
     * @brief   Renders the object
     */
    virtual void render() override;

private:

    /**
     * @brief   Sets color and material values for rendering
     */
    void setColorMaterial(Color &ambient, Color &diffuse, Color &specular, float shininess);

    /// The material descriptions
    vector<Material::Ptr>   m_materials;

    /// Lists of faces that share a material
    MaterialFaceLists   m_matFaceLists;

    /// Texture coordinate buffer
    FloatArray              m_textureCoords;



};

}

#endif /* TEXTUREDMESH_HPP_ */
