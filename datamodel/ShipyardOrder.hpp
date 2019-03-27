/**
 * ShipyardOrder.hpp
 *
 *  Created on: 27.03.2019
 *      Author: gvogt
 * 
 * Represents an order to build a Shipyard on a planet a player can give during the active phase
 */
#ifndef __SHIPYARDORDER_HPP_
#define __SHIPYARDORDER_HPP_



#include <string>
#include <list>
#include <memory>

#include "Planet.hpp"

namespace asteroids
{

        class Planet; 
        
	class ShipyardOrder
	{

        public:
        using Ptr = std::shared_ptr<ShipyardOrder>;

        /**
         * @brief Constructor
         * @param P1 The planeet on which a Shipyard should be built
         */
        ShipyardOrder(std::shared_ptr<Planet> P1);

        /**
         * @brief returns Planet
         * @return The planet on which a Shipyard should be built
         */
        std::shared_ptr<Planet> getPlanet()
        {
                return m_P;
        }

        private:
        /**
         * @brief The planeet on which a Shipyard should be built
         */
        std::shared_ptr<Planet> m_P;


	};
}

#endif