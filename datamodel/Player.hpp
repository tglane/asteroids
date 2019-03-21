#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_
/*
 * Player.hpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 */
#include <list>
#include <string>
#include <memory>

#include "Planet.hpp"
#include "MoveOrder.hpp"
#include "MineOrder.hpp"
#include "ShipOrder.hpp"

namespace asteroids
{

class Planet;


	//using Planet::Ptr = std::shared_ptr<Planet>;

class Player
{
public:
	using Ptr = std::shared_ptr<Player>;

	Player();

	Player(int identity, int rubin = 0, int ships = 0);

	int getShips();

	std::list<std::shared_ptr<Planet>> getPlanets();

	int getIdentity();

	int getRubin();

	void addRubin(int rubin);

	bool delRubin(int rubin);

private:

	int m_identity;
	int m_rubin;
	//freie Raumschiffe?
	int m_ships;
	std::list<std::shared_ptr<Planet>> m_planets;

	std::list<MoveOrder::Ptr> m_moveOrders;
	std::list<MineOrder::Ptr> m_mineOrders;
	std::list<ShipOrder::Ptr> m_shipOrders;

};
}

#endif