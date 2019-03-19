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
#include "Planet.hpp"

namespace asteroids
{

class Planet;

class Player
{
public:

	Player();

	Player(int identity, int rubin = 0, int ships = 0);

	int getShips();

	std::list<Planet*> getPlanets();

	int getIdentity();

	int getRubin();

	void addRubin(int rubin);

	bool delRubin(int rubin);

private:

	int m_identity;
	int m_rubin;
	//freie Raumschiffe?
	int m_ships;
	std::list<Planet*> m_planets;
};
}

#endif