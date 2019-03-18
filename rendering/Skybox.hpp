/*
 *  Skybox.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */
#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <string>
using std::string;

#include "io/TextureFactory.hpp"
#include "view/Camera.hpp"
#include "util/SharedArray.hpp"
#include "Renderable.hpp"

#include <memory>

namespace asteroids
{

/**
 * @brief Renders a Skybox with given dimensions
 */
class Skybox
{
public:
    
    using TexArray = shared_array<Texture::Ptr>;
    using Ptr = std::shared_ptr<Skybox>;

    /**
     * @brief Contructor. Build a skybox with the given dimensions
     *
     * @param   width   The width of the box
     * @param   files   6 filenames containing the texture image data
     */
    Skybox(int width, string files[6]);

    Skybox() = delete;

    /**
     * @brief Dtor.
     */
    virtual ~Skybox() = default;

    /**
     * @brief Renders the Skybox
     */
    virtual void render(Camera& cam);

private:

    /// An array containing the sykbox textures
    TexArray        m_textures;

    /// The with of the Skybox
    int             m_width;
};

} // namespace asteroids

#endif /* SKYBOX_H_ */
