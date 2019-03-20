/**
 * Order.hpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 * 
 * Represents an order which a player gives during the active phase
 */
#ifndef __Order_HPP_
#define __Order_HPP_


#include <string>
#include <list>

#include "Planet.hpp"
#define MINE 1

namespace asteroids
{
	class Order
	{
        public:
        /**
         * @brief Can give the information of which type this Order is
         */
        int type;
        

	};
}

#endif