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
class MineOrder;
class ShipOrder;
class MoveOrder;

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

	void setPlayerName(std::string name);

	std::string getPlayerName();

	std::list<std::shared_ptr<ShipOrder>> getListShipOrder();

	void putListShipOrder(std::shared_ptr<ShipOrder> NewShipOrder);

	std::list<std::shared_ptr<MineOrder>> getListMineOrder();

	void putListMineOrder(std::shared_ptr<MineOrder> NewMineOrder);

	void ClearOrderListInPlayer();

private:

	std::string m_name;
	int m_identity;
	int m_rubin;
	//freie Raumschiffe?
	int m_ships;
	std::list<std::shared_ptr<Planet>> m_planets;

	std::list<std::shared_ptr<MoveOrder>> m_moveOrders;
	std::list<std::shared_ptr<MineOrder>> m_mineOrders;
	std::list<std::shared_ptr<ShipOrder>> m_shipOrders;

};
}

#endif