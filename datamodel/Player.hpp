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
#include <iostream>
#include <iterator>

#include <map>

#include "Planet.hpp"
#include "MoveOrder.hpp"
#include "MineOrder.hpp"
#include "ShipOrder.hpp"
#include "ShipyardOrder.hpp"

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
	Player(int identity, int rubin = 3000, int ships = 0, std::string player_name = "Mr. Example");

	/**
	 * @brief returns the number of ships
	 * @return int number of ships
	 */
	int getShips();

	/**
	 * @brief sets number of Ships by given Number
	 * Obacht, da nicht auf Planeten abgebildet!
	 */
	void setShips(int ships);

	/**
	 * @brief deletes a given number of ships ships
	 * Obacht, da nicht auf Planeten abgebildet!
	 */
	void delShips(int ShipNumber);

	/**
	 * @brief returns number of mines in our ownership
	 */
	int getMines();

	/**
	 * @brief sets number of mines by given number
	 * Obacht, nicht auf Planeten abgebildet!
	 */
	void setMines(int mines);

	/**
	 * @brief sets/replaces the list containing my planets
	 */
	void setPlanetsList(std::list<std::shared_ptr<Planet>> planets);

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
	 * @brief set the identty of the player
	 * @param ind id to set
	 */
	 void setIdentity(int id);

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
	 * @brief increase ship-counter
	 */
	void incShips(int Ships);

	/**
	 * @brief increase mines-counter
	 */
	void incMines();

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

	/**
	 * @brief returns the ship order list
	 * @return std::list<std::shared_ptr<ShipOrder>> ship order list
	 */
	std::list<std::shared_ptr<ShipOrder>> getListShipOrder();

	/**
	 * @brief add a ship order to the list
	 * @param std::shared_ptr<ShipOrder> new ship order
	 */
	void putListShipOrder(std::shared_ptr<ShipOrder> NewShipOrder);

	/**
	 * @brief returns mine order list
	 * @return std::list<std::shared_ptr<MineOrder>> mine order list
	 */
	std::list<std::shared_ptr<MineOrder>> getListMineOrder();

	/**
	 * @brief add a mine order to the list
	 * @param std::shared_ptr<MineOrder> new mine order
	 */
	void putListMineOrder(std::shared_ptr<MineOrder> NewMineOrder);

	/**
	 * @brief returns the move order list
	 * @return std::list<std::shared_ptr<MoveOrder>> move order list
	 */
	std::list<std::shared_ptr<MoveOrder>> getListMoveOrder();

	/**
	 * @brief add a move order to the list
	 * @param std::shared_ptr<MoveOrder> new move order
	 */
	void putListMoveOrder(std::shared_ptr<MoveOrder> newMoveOrder);

	/**
	 * @brief add a Shipyard order to the list
	 * @param std::shared_ptr<ShipyardOrder> new Shipyard order
	 */
	void putListShipyardOrder(std::shared_ptr<ShipyardOrder> newShipyardOrder);

	/**
	 * @brief returns the Shipyard order list
	 * @return std::list<std::shared_ptr<ShipyardOrder>> Shipyard order list
	 */
	std::list<std::shared_ptr<ShipyardOrder>> getListShipyardOrder();


	/**
	 * @brief clears the ship, mine and move order list
	 */
	void ClearOrderListInPlayer();

	std::list<std::shared_ptr<Planet>> getListOfPLanets();

	void RemovePlaneteFromList(Planet::Ptr Planet);

	void PrintPlanetsList();

	/**
	 * @brief updates number of rubin and ships by the values of the planets in our ownership
	 *
	void updateResources();*/

private:

	std::string m_name;
	int m_identity;
	int m_rubin;
	//freie Raumschiffe?
	int m_ships;
	int m_mines;
	std::list<std::shared_ptr<Planet>> m_planets;

	std::list<std::shared_ptr<MoveOrder>> m_moveOrders;
	std::list<std::shared_ptr<MineOrder>> m_mineOrders;
	std::list<std::shared_ptr<ShipOrder>> m_shipOrders;
	std::list<std::shared_ptr<ShipyardOrder>> m_shipyardOrders;


};
}


#endif
