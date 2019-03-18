/*
 *  Texture.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>
using std::string;

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "io/BitmapReader.hpp"

namespace asteroids
{

class Texture
{
public:
    using Ptr = std::shared_ptr<Texture>;

    /**
     * @brief   Initializes a texture with given date. Class
     *          will release the data.
     *
     * @param   pixels  The image data of the texture. Pixel
     *                  aligned, three bytes per pixel
     * @param   width   The image width
     * @param   height  The image height
     */
    Texture(BitmapReader::Ptr pixels, int width, int height);

    /**
     * @brief   Copy ctor.
     */
    Texture(const Texture &other);

    Texture() = delete;

    /**
     * @brief   Dtor.
     */
    virtual ~Texture() = default;

    /**
     * @brief   Bind the texture to the current OpenGL context
     */
    void bind() const { glBindTexture(GL_TEXTURE_2D, m_texIndex);}

private:

    /**
     * @brief   Does all the OpenGL stuff to make it avialable for
     *          rendering.
     */
    void upload();

    /// The width of the texture in pixels
    int                 m_width;

    /// The height of the texture in pixels
    int                 m_height;

    /// The aligned pixel data. Three bytes per pixel (r,g,b)
    BitmapReader::Ptr   m_pixels;

    /// The texture index of the texture
    GLuint              m_texIndex;

    friend class TextureFactory;
};

}
#endif /* TEXTURE_HPP_ */
