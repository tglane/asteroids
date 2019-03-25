/**
 * Battle.cpp
 *
 *  Created on: 21.03.2019
 *      Author: gvogt
 */

#include "Battle.hpp"

namespace asteroids{

    Battle::Battle(std::shared_ptr<Planet> location, Player::Ptr p1, Player::Ptr p2, int nShips1, int nShips2, bool FightResultInvader1):
    m_location(location), m_player1(p1), m_player2(p2), m_numberShips1(nShips1), m_numberShips2(nShips2),
    m_numberShipsLost1(0),m_numberShipsLost2(0), FightResultInvader(FightResultInvader1)
    {

    }

}
