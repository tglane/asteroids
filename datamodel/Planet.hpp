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

#include "Player.hpp"

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
		 * @param rubin optional param =0 if not given, how many rubin the planet gives to the owner
		 * @param mines optional param =0 if not given, how many mines are build on the planet
		 *
		 */
		Planet(std::string name, int posx, int posy, int rubin = 0, int mines = 0);

		/**
		 * @brief gives the number of ships on this planet
		 * @return number of ships
		 */
		int getShips();

		std::string getName();

		/**
		 * @brief add ships to the planet
		 * @param ships number of ships
		 * @return void
		 */
		void addShips(int ships);

		/**
		 * @brief deletes ships on the planet
		 * @param ships number of ships
		 *
		 */
		bool delShips(int ships);

		void addNeighbour(Planet::Ptr neighbour);

		void setOwner(std::shared_ptr<Player> owner);

		int getRubin();

		int getMines();

		void addMines(int mines); //erhöht rubin

		std::list<Planet::Ptr> getNeighbours();

		int getPosX();

		int getPosY();

		std::string getName();


	private:
		std::string m_name;
		std::list<Planet::Ptr> m_neighbours;
		int m_mines;
		int m_rubin;
		int m_ships;
		std::shared_ptr<Player> m_owner;

		int m_posx;
		int m_posy;	

	};
}

#endif
