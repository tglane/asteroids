/**
 * Moveorder.cpp
 *
 *  Created on: 20.03.2019
 *      Author: gvogt
 */

#include "MoveOrder.hpp"

namespace asteroids{

    MoveOrder::MoveOrder(Planet::Ptr orig, Planet::Ptr dest, int numShip):
    origin(orig), destination(dest), numberShips(numShip), type(MOVE)
    {
        
    }

}