/*
 *  Texture.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Texture.hpp"

#include <iostream>

namespace asteroids
{

Texture::Texture(BitmapReader::Ptr pixels, int width, int height)
    : m_width(width), m_height(height),  m_pixels(pixels)
{
    m_texIndex = 0;
    // Upload texture
    upload();
}

Texture::Texture(const Texture &other)
{
    // Copy data
    m_width = other.m_width;
    m_height = other.m_height;
    m_pixels = other.m_pixels;
    m_texIndex = other.m_texIndex;

}

void Texture::upload()
{
    glEnable(GL_TEXTURE_2D);
    // Create new texure list
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                              // Set correct alignment
    glGenTextures(1, &m_texIndex);

    // Bind texture, setup parameters and upload it
    // to video memory
    glBindTexture(GL_TEXTURE_2D, m_texIndex);                           // Bind texture
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);      // Set repeating and filetering
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);           // Do not apply lighting

    // Upload texture
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            m_width,
            m_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pixels.get());

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "GL EROOR: " << err << std::endl;
    }
}

}
