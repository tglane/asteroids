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

class MineOrder;
class ShipOrder;
class MoveOrder;

class Player
{
public:
	using Ptr = std::shared_ptr<Player>;

	/**
	 * @brief default constructor
	 * calls default constructor of m_planets list
	 */
	Player();

	/**
	 * @brief constructor
	 * @param int identity
	 * @param int rubin = 0
	 * @paran int ships = 0
	 */
	Player(int identity, int rubin = 0, int ships = 0);

	/**
	 * @brief returns the number of ships
	 * @return int number of ships
	 */
	int getShips();

	/**
	 * @brief adds planet to the planetslist of the player
	 * @param shared ptr planet to add
	 */
	void addPlanet(std::shared_ptr<Planet> planet);

	/**
	 * @brief returns a list with the planets this player owns
	 * @return std::list which contains shared_ptr to the planets
	 */
	std::list<std::shared_ptr<Planet>> getPlanets();

	/**
	 * @brief returns the identity of this player
	 * @return int the identity
	 */
	int getIdentity();

	/**
	 * @brief returns the number of rubin this player owns
	 * @return int number of return
	 */
	int getRubin();

	/**
	 * @brief increases the rubin this player owns
	 * @param int rubin to add
	 *
	 */
	void addRubin(int rubin);

	/**
	 * @brief decrease the rubin this player owns
	 * @param int rubin to subtract
	 */
	bool delRubin(int rubin);

	/**
	 * @brief set the name of this player
	 * @param string name of this player
	 */
	void setPlayerName(std::string name);

	/**
	 * @brief returns the name of this player
	 * @return std::string name
	 */
	std::string getPlayerName();

	std::list<std::shared_ptr<ShipOrder>> getListShipOrder();

	void putListShipOrder(std::shared_ptr<ShipOrder> NewShipOrder);

	std::list<std::shared_ptr<MineOrder>> getListMineOrder();

	void putListMineOrder(std::shared_ptr<MineOrder> NewMineOrder);

	void putListMoveOrder(std::shared_ptr<MoveOrder> newMoveOrder);

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
