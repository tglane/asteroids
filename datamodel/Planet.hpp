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

#include "Player.hpp"

namespace asteroids
{

	class Player;

	class Planet
	{

	public:

		//liste leer initialisieren, player auch)
		Planet(std::string name, int posx, int posy, int rubin = 0, int mines = 0);

		int getShips();

		void addShips(int ships);

		bool delShips(int ships);

		void addNeighbour(Planet* neighbour);

		void setOwner(Player* owner);

		int getRubin();

		int getMines();

		void addMines(int mines); //erhöht rubin

		std::list<Planet*> getNeighbours();

		int getPosX();

		int getPosY();


	private:
		std::string m_name;
		std::list<Planet*> m_neighbours;
		int m_mines;
		int m_rubin;
		int m_ships;
		Player* m_owner;

		int m_posx;
		int m_posy;	

	};
}

#endif