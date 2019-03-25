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

namespace asteroids
{
        class Planet;

	class MoveOrder
	{
        public:
        using Ptr = std::shared_ptr<MoveOrder>;

        /**
         * @brief Constructor
         * @param origin1 The planet from which the ships are coming
         * @param destination1 The planet to which the ships are going
         * @param numberShips1 The number of ships that is supposed to move to the planet
         */
        MoveOrder(std::shared_ptr<Planet> origin1, std::shared_ptr<Planet> destination1, int numberShips1);

        /**
         * @brief returns the origin planet
         * @return The planet from which the moved ships are coming
         */
        std::shared_ptr<Planet> getOrigin()
        {
                return m_origin;
        }

        /**
         * @brief returns the destination planet
         * @return The planet to which the moved ships are going
         */
        std::shared_ptr<Planet> getDestination()
        {
                return m_destination;
        }

        /**
         * @brief returns the originally moved number of ships
         * @return the originally ordered number of shhips to be moved
         */
        int getNumberShips()
        {
                return m_numberShips;
        }

        private:
        /**
         * @brief The Planet to which the ships are moving
         */
        std::shared_ptr<Planet> m_destination;

        /**
         * @brief The planet from which the ships are coming
         */
        std::shared_ptr<Planet> m_origin;

        /**
         * @brief number of ships to be moved 
         */
        int m_numberShips;

	};
}

#endif
