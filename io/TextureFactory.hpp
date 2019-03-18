/*
 *  TextureFactory.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */
#ifndef TEXTUREFACTORY_H_
#define TEXTUREFACTORY_H_

#include <string>
using std::string;

#include <map>

#include "rendering/Texture.hpp"

namespace asteroids
{

class TextureFactory
{
protected:

    /**
     * @brief Constructor
     */
    TextureFactory();

    /**
     * @brief Destructor
     */
    virtual ~TextureFactory();

    static std::map<string, Texture::Ptr> m_loadedTextures;

public:

    /**
     * @brief   Returns a new texture if the file contains readable
     *          image data or a null point if the file couldn't be parsed
     */
    auto getTexture(const string& filename) -> Texture::Ptr;

    /**
     * @brief Sets the base path for loading Textures
     */
    static void setBasePath(const string& base);

    /**
     * @brief   Returns the singleton instance
     */
    static TextureFactory& instance();

    static string m_basePath;

    
};

}
#endif /* TEXTUREFACTORY_H_ */
