/**
 * Mineorder.cpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 */

#include "MineOrder.hpp"

namespace asteroids{

    MineOrder::MineOrder(Planet::Ptr P1):P(P1)
    {
        type = MINE;
    }

}

