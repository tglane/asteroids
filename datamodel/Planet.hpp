#ifndef __PLANET_HPP_
#define __PLANET_HPP_
/*
 * Planet.hpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 *
 *  Represents a player
 */

#include <string>
#include <list>
#include <memory>
#include <iostream>

namespace asteroids
{
	class Player;
	class Planet
	{

	public:

		using Ptr = std::shared_ptr<Planet>;

		/**
		 * @brief constructor
		 *
		 * @param name of the planet
		 *
		 * @param posx position on x axis
		 * @param posy position on y axis
		 *
		 * @param rubin optional param =0 how many rubin the planet gives to the owner by each round
		 * @param mines optional param =0 how many mines are built on the planet
		 * @param rubinLeft optional param = 20 how many rubin is left on this planet
		 *
		 */
		Planet(std::string name, int posx, int posy/*, int rubin = 0*/, int mines = 0, int rubinLeft = 20000);

		/**
		 * @brief gives the number of ships on this planet
		 * @return number of ships
		 */
		int getShips();

		/**
		 * @brief add ships to the planet
		 * @param ships number of ships
		 * @return void
		 */
		void addShips(int ships);

		/**
		 * @brief deletes ships on the planet
		 * @param ships number of ships
		 * @return boolean if deleting given number of ships was successful or not
		 */
		bool delShips(int ships);

		/**
		 * @brief adds given Planet as Neighbour
		 * @param neighbour: in Planet defined sharedPointer pointing our Neighbour
		 * @return void
		 */
		void addNeighbour(Planet::Ptr neighbour);

		/**
		 * @brief sets a Player as the Owner of this planet
		 * @param owner shared Pointer to our new owner
		 * @return void
		 */
		void setOwner(std::shared_ptr<Player> owner);

		/**
		 *@brief sets a Player who wants to colonialize this planet
		 *@param shared ptr to the invader
		 */
		void setInvader(std::shared_ptr<Player> invader);

		/**
		 * @brief sets number of ships from the owner on this planet
		 */
		void setShips(int ships);

		/**
		 * @brief sets number of mines from the owner on this planet
		 */
		void setMines(int mines);

		/**
		 * @brief returns the owner of this planet
		 * @return shared_ptr to the owner
		 */
		std::shared_ptr<Player> getOwner();

		/**
		 * @brief returns the player who wants to colonialise this planet
		 * @return shared_ptr to the invader
		 */
		std::shared_ptr<Player> getInvader();

		/**
		 * @brief sets the number of the invaderships
		 * @param int number of invaderships
		 */
		void setInvaderShips(int invaderShips);

		/**
		 * @brief add Ships to the amount of InvaderShips
		 * @param int invaderShips ships to add
		 */
		void addInvaderShips(int invaderShips);

		/**
		 * @brief returns the number of the invaderships
		 * @return int
		 */
		int getInvaderShips();

		/**
		 * @brief returns the number of rubin the owner earn from this planet
		 * @return int number of rubin
		 *
		int getRubin();*/

		/**
		 * @brief return the number of mines on this planet
		 * @return int number of mines
		 */
		int getMines();

		/**
		 * @brief adds Mines to this Planet
		 * @param number of mines to add
		 */
		void addMines(int mines); //erhöht rubin

		/**
		 * @brief returns a std::list which contains smartpointer to the neighbour planets
		 * @return std::list<Planet::Ptr> contains smartpointer to the neighbour planets
		 */
		std::list<Planet::Ptr> getNeighbours();

		/**
		 * @brief return position on x-axis
		 * @return int position
		 */
		int getPosX();

		/**
		 * @brief return position on y-axis
		 * @return int position
		 */
		int getPosY();

		/**
		 * @brief return the name of this planet
		 * @return std::string name of this planet
		 */
		std::string getName();

		int getMinesHidden();
		/**
		 * @brief increment m_minesHidden (ordered Mines)
		 */
		void setMinesHidden();

		/**
		 * @return m_minesBuild (mines which are already built (not only ordered))
		 */
		int getMinesBuild();


		void setMinesBuild();

		/**
		 * @brief set built mines to the given number
		 * @param minesBuilt
		 * @return void
		 */
		void setMinesBuilt(int minesBuilt);


		/**
		 * @brief returns number of ordered ships to build on this planet
		 */
		int getShipsOrdered();

		/**
		 * @brief increments numbe rof ordered ships to build on this planet
		 */
		void incShipsOrdered();

		void resetShipsOrdered();

		void resetMinesHidden() { m_minesHidden = 0; }

		/**
		 * @brief reduce rubin left on this planet
		 * @param how many each mine earns
		 * @return how many rubin this planet gave this round
		 */
		int subtractEarnings(int mineRubin);

		/**
		 * @sets m_rubinLeft to the given value
		 */
		void setRubinLeft(int rubinLeft);

		/**
		 * @brief returns how many rubin is left on this planet
		 * @return int m_rubinLeft
		 */
		int getRubinLeft();
		/**
		 * @brief calculates current number of rubin this planet produce by each round
		 * @param mineRubin how many rubin each mine should give
		 */
		int calculateEarnings(int mineRubin);


		bool getShipyardBuilt();

		void buildShipyard();


	private:
		std::string m_name;

		std::list<Planet::Ptr> m_neighbours;

		bool m_shipyardBuilt;

		int m_mines;
		/**
		 * @brief mines already built
		 */
		int m_minesbuild;
		/**
		 * @brief ordered mines
		 */
		int m_minesHidden;

		//int m_rubin;
		int m_rubinLeft;
		int m_ships;
		int m_invaderShips;
		int m_ships_ordered;

		std::shared_ptr<Player> m_owner;
		std::shared_ptr<Player> m_invader;

		int m_posx;
		int m_posy;

	};
}

#endif


