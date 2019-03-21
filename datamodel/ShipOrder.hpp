/**
 * ShipOrder.hpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 * 
 * Represents an order to build a Ship on a planet a player can give during the active phase
 */
#ifndef __SHIPORDER_HPP_
#define __SHIPORDER_HPP_



#include <string>
#include <list>
#include <memory>

#include "Planet.hpp"
#include "Order.hpp"

namespace asteroids
{

	class ShipOrder : public Order
	{
        using Ptr = std::shared_ptr<ShipOrder>;

        public:
        /**
         * @brief Constructor
         * @param P1 The planeet on which a Ship should be built
         */
        ShipOrder(Planet::Ptr P1);


        /**
         * @brief returns Planet
         * @return The planet on which a ship should be built
         */
        Planet::Ptr getPlanet()
        {
                return m_P;
        }

        private:
        /**
         * @brief The planet on which a ship should be built
         */
        Planet::Ptr m_P;

	};
}

#endif