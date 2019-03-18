/*
 *  Materials.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */
#ifndef MATERIALS_HPP_
#define MATERIALS_HPP_

#include <list>
#include <memory>

using std::list;

#include "Texture.hpp"

namespace asteroids
{

class Texture;

struct Color
{
    Color() : r(125), g(125), b(125) {};
    float r;
    float g;
    float b;
};

struct TexCoord
{
    float u;
    float v;
};

struct Material
{
    using Ptr = std::shared_ptr<Material>;

    Color               m_ambient;
    Color               m_diffuse;
    Color               m_specular;
    float               m_shininess;
    Texture::Ptr        m_texture;
};

struct MaterialFaceList
{
    using Ptr = std::shared_ptr<MaterialFaceList>;
    int         m_matIndex;
    list<int>   m_faces;
};

typedef list<MaterialFaceList::Ptr> MaterialFaceLists;

} // namespace asteroids

#endif /* MATERIALS_HPP_ */
