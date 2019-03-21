/**
 * MineOrder.hpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 * 
 * Represents an order to build a mine on a planet a player can give during the active phase
 */
#ifndef __MINEORDER_HPP_
#define __MINEORDER_HPP_



#include <string>
#include <list>
#include <memory>

#include "Planet.hpp"

namespace asteroids
{

	class MineOrder
	{

        using Ptr = std::shared_ptr<MineOrder>;

        public:
        /**
         * @brief Constructor
         * @param P1 The planeet on which a mine should be built
         */
        MineOrder(Planet::Ptr P1);

        /**
         * @brief returns Planet
         * @return The planet on which a mine should be built
         */
        Planet::Ptr getPlanet()
        {
                return m_P;
        }

        private:
        /**
         * @brief The planeet on which a mine should be built
         */
        Planet::Ptr m_P;


	};
}

#endif