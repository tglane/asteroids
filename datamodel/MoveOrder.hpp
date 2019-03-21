/**
 * MoveOrder.hpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 * 
 * Represents an order to Move a few ships fomr one planet to another
 */
#ifndef __MOVEORDER_HPP_
#define __MOVEORDER_HPP_



#include <string>
#include <list>
#include <memory>

#include "Planet.hpp"
#include "Order.hpp"

namespace asteroids
{



	class MoveOrder : public Order
	{
        public:


		using Ptr = std::shared_ptr<MoveOrder>;
        /**
         * @brief Constructor
         * @param origin1 The planet from which the ships are coming
         * @param destination1 The planet to which the ships are going
         * @param numberShips1 The number of ships that is supposed to move to the planet
         */
        MoveOrder(Planet::Ptr origin1, Planet::Ptr destination1, int numberShips1);

        /**
         * @brief The Planet to which the ships are moving
         */
        Planet::Ptr destination;

        /**
         * @brief The planet from which the ships are coming
         */
        Planet::Ptr origin;

        /**
         * @brief number of ships to be moved 
         */
        int numberShips;

        /**
         * @brief Can give the information of which type this Order is
         */
        int type;

	};
}

#endif
