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

	class Planet
	{

		//liste leer initialisieren, player auch)
		Planet(const std::string name, int rubin = 0, int mines = 0);

		int getShips();

		void addShips(int ships);

		boolean delShips(int ships);

		void addNeighbour(Planet neighbour);

		void setOwner(Player owner);

		int getRubin();

		int getMines();

		void addMines(int mines); //erhöht rubin

		std::list<Planet> getNeighbours();





	private:
		const std::string m_name;
		const std::list<Planet> m_neighbours;
		int m_mines;
		int m_rubin;
		Player m_owner;





	};
}


