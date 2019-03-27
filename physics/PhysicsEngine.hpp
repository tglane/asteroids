/*
 *  PhysicsEngine.cpp
 *
 *  @date 19.01.2019
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2019 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#ifndef PHYSICSENGINE_HPP_
#define PHYSICSENGINE_HPP_

#include <map>
#include <memory>

#include "PhysicalObject.hpp"
#include "ParticleEngine.hpp"
#include "rendering/Bullet.hpp"
#include "rendering/SpaceCraft.hpp"
#include "BasePhysicsEngine.hpp"


using std::map;


namespace asteroids
{

/**
 * @brief   Class that calculates the position of all present objects
 *          and implements collision detection. Handles particle
 *          effects like explosions.
 */
    class PhysicsEngine : public BasePhysicsEngine, public Renderable
    {
    public:

        using Ptr = std::shared_ptr<PhysicsEngine>;

        /**
         * @brief   Ctor.
         */
        PhysicsEngine() = default;

        /**
         * @brief   Dtor.
         */
        virtual ~PhysicsEngine() = default;

        /**
         * @brief   Renders all objects and particle effects
         */
        void render();

        /**
         * @brief   The engine's main loop
         */
        bool process(int elapsed_time);

    void process_collisions(int id_one, int id_two);
    int check_id_type(int id_to_check);


        /// Getter for current highest IDs
        int get_curr_bull_id() { return (++curr_bull_id) % 65536; }
        int get_curr_miss_id() { return (++curr_miss_id) % 65536; }
        int get_curr_dest_id() { return curr_dest_id; }

    private:

        ParticleEngine               m_particles;

        /// Current highest id of asteroids and bullets
        int curr_bull_id = 1;
        int curr_miss_id = 1;
        int curr_dest_id = 1;
        int curr_player_id = 1;

    };

} /* namespace asteroids */
#endif /* PHYSICSENGINE_HPP_ */
