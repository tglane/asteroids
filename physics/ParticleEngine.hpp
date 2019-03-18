/* Copyright (C) 2011 Uni Osnabrück
 * This file is part of the LAS VEGAS Reconstruction Toolkit,
 *
 * LAS VEGAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LAS VEGAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

/**
 * ParticleEngine.h
 *
 *  @date Jan 10, 2013
 *  @author Thomas Wiemann
 */

#ifndef PARTICLEENGINE_H_
#define PARTICLEENGINE_H_

#include <list>

#include "rendering/Renderable.hpp"
#include "rendering/ParticleEffect.hpp"

namespace asteroids
{

/**
 * @brief   Class to render particle effects
 */
class ParticleEngine : public Renderable
{
public:

    /**
     * @brief   Ctor
     */
    ParticleEngine() = default;

    /**
     * @brief   Adds an @ref ParticleEffect to the engine
     * @param   e   The new effect
     */
    void addEffect(ParticleEffect::Ptr e);

    /**
     * @brief   Renders all particle effects
     */
    void render();

    /**
     * @brief   Ctor.
     */
    virtual ~ParticleEngine() = default;

    /**
     * @brief   Main loop. Checks if an effect has ended and updates to
     *          list of active effects
     */
    void update();

private:

    /// List of active @ref ParticleEffects
    std::list<ParticleEffect::Ptr>     m_effects;
};

} /* namespace asteroids */
#endif /* PARTICLEENGINE_H_ */
