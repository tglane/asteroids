/**
 * Order.hpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 * 
 * Represents an order which a player gives during the active phase
 */
#ifndef __ORDER_HPP_
#define __ORDER_HPP_


#include <string>
#include <list>

#include "Planet.hpp"
#define MINE 1
#define SHIP 2
#define MOVE 3

namespace asteroids
{
	class Order
	{
        public:

        using Ptr = std::shared_ptr<Order>;

        virtual int getType()
        {
                return m_type;
        }

        private:
        /**
         * @brief Can give the information of which type this Order is
         */
        int m_type;
        

	};
}

#endif