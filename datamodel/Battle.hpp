/**
 * Battle.hpp
 *
 *  Created on: 21.03.2019
 *      Author: gvogt
 * 
 * Represents a battle that still has to take place
 */
#ifndef __BATTLE_HPP_
#define __BATTLE_HPP_



#include <string>
#include <list>
#include <memory>

#include "Planet.hpp"
#include "Player.hpp"

namespace asteroids
{

        class Planet; 
        
	class Battle
	{

        public:
        using Ptr = std::shared_ptr<Battle>;

        /**
         * @brief constructor
         * @param location Location of the battle
         * @param p1 player 1 of the battle
         * @param p2 player 2 of the battle
         * @param nShips1 Number of ships which player 1 has available in the battle
         * @param nShips2 Number of ships which player 2 has available in the battle 
         */
        Battle(std::shared_ptr<Planet> location, Player::Ptr p1, Player::Ptr p2, int nShips1, int nShips2, bool FightResultInvader);

        public:

        /**
         * @brief Planet at which the battle is taking place 
         */
        std::shared_ptr<Planet> m_location;

        /**
         * @brief Player 1 of the fight
         */
        Player::Ptr m_player1;

        /**
         * @brief Player 2 of the fight
         */
        Player::Ptr m_player2;

        /**
         * @brief Number of Ships player 1 has
         */
        int m_numberShips1;

        /**
         * @brief Number of Ships player 2 has
         */
        int m_numberShips2;

        /**
         * @brief Number of Ships player 1 has lost
         */
        int m_numberShipsLost1;

        /**
         * @brief Number of Ships player 2 has lost
         */
        int m_numberShipsLost2;


        bool FightResultInvader;


	};
}

#endif