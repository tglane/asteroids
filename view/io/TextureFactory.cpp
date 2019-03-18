/*
 *  TextureFactory.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "TextureFactory.hpp"
#include "ReadPPM.hpp"
#include "ReadTGA.hpp"
#include "ReadJPG.hpp"
#include "BitmapReader.hpp"

#include <iostream>
#include <memory>
using std::cout;
using std::endl;

namespace asteroids
{

std::map<string, Texture::Ptr> TextureFactory::m_loadedTextures;

string TextureFactory::m_basePath;

TextureFactory::TextureFactory()
{
    // TODO Auto-generated constructor stub

}

TextureFactory::~TextureFactory()
{
    // TODO Auto-generated destructor stub
}

TextureFactory& TextureFactory::instance()
{
    // Just crate one instance
    static TextureFactory instance;
    return instance;
}

void TextureFactory::setBasePath(const string& base)
{
	m_basePath = base;
}

auto TextureFactory::getTexture(const string& filename) -> Texture::Ptr
{
    // A texture object
    Texture::Ptr tex = 0;

    string tex_filename = m_basePath + filename;

    std::map<string, Texture::Ptr>::iterator it = m_loadedTextures.find(tex_filename);
    if(it == m_loadedTextures.end())
    {
        // Texture data
        int width = 0;
        int height = 0;
        BitmapReader::Ptr data = 0;

        std::shared_ptr<BitmapReader> reader;

        // Get file extension
        if(filename.substr(filename.find_last_of(".") + 1) == "ppm")
        {
            std::shared_ptr<ReadPPM> read_ppm(new ReadPPM(tex_filename));
            reader = std::static_pointer_cast<BitmapReader>(read_ppm);
        }
        else if(filename.substr(filename.find_last_of(".") + 1) == "tga")
        {
            std::shared_ptr<ReadTGA> read_tga(new ReadTGA(tex_filename));
            reader = std::static_pointer_cast<BitmapReader>(read_tga);
        }
        else if(filename.substr(filename.find_last_of(".") + 1) == "jpg")
        {
            std::shared_ptr<ReadJPG> read_jpg(new ReadJPG(tex_filename));
            reader = std::static_pointer_cast<BitmapReader>(read_jpg);
        }

        if(reader)
        {
            data = reader->getPixels();
            width = reader->getWidth();
            height = reader->getHeight();
        }

        // Check data and create new texture if possible
        if(data != 0 && width != 0 && height != 0)
        {
            tex = Texture::Ptr( new Texture(data, width, height));
            m_loadedTextures[tex_filename] = tex;
        }
        else
        {
            cout << "TextureFactory: Unable to read file " << tex_filename << "." << endl;
        }

        return tex;
    }
    else
    {
        return m_loadedTextures[tex_filename];
    }
}

}

