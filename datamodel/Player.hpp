/*
 * Player.hpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 */
#include <list>
#include <string>
#include "../datamodel/Planet.hpp"

namespace asteroids

{
class Player
{
public:


	Player (const int identity, std::list<Planet> m_planets, int rubin = 0, int ships = 0);

	int getShips();

	std::list<Planet> getPlanets();

	int getIdentity();

	int getRubin();

	void addRubin(int rubin);

	bool delRubin(int rubin);

private:

	int m_identity;
	int m_rubin;
	//freie Raumschiffe?
	int m_ships;
	std::list<Planet> m_planets;
	//Spacecraft m_spacecraft;



	};
}
