/*
 *  Renderable.hpp
 *
 *  Created on: Nov. 24 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __RENDERABLE_HPP__
#define __RENDERABLE_HPP__

#include <memory>

namespace asteroids
{
    

/**
 * @brief Base interface for all renderable objects.
 * 
 */
class Renderable 
{
public:

    using Ptr = std::shared_ptr<Renderable>;

    /**
     * @brief   Overwrite this method and implement
     *          the specific rendering code in it.
     * 
     */
    virtual void render() = 0;
};

}
#endif