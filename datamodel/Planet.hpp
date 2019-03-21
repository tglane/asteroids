#ifndef __PLANET_HPP_
#define __PLANET_HPP_
/*
 * Planet.hpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
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

		//liste leer initialisieren, player auch)
		Planet(std::string name, int posx, int posy, int rubin = 0, int mines = 0);

		std::string getName();

		int getShips();

		void addShips(int ships);

		bool delShips(int ships);

		void addNeighbour(Planet* neighbour);

		void setOwner(std::shared_ptr<Player> owner);

		int getRubin();

		int getMines();

		void addMines(int mines); //erhöht rubin

		std::list<Planet::Ptr> getNeighbours();

		int getPosX();

		int getPosY();


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