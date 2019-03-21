/*
 * Planet.cpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 */


#include "Planet.hpp"
#include "Player.hpp"
#include <string>


namespace asteroids
{

Planet::Planet(std::string name, int posx, int posy, int rubin, int mines) : m_neighbours() 
{
	m_posx = posx;
	m_posy = posy;
	m_name = name;
	m_rubin = rubin;
	m_mines = mines;
	m_ships = 0;
	m_owner = NULL;
}


int Planet::getShips() 
{
	return m_ships;
}

void Planet::addShips(int ships) 
{
	m_ships += ships;
}

bool Planet::delShips(int ships) 
{
	if (ships > m_ships)
	{
		return false;
	}
	else
	{
		m_ships -= ships; 
		return true;
	} 
}

void Planet::addNeighbour(Planet::Ptr neighbour)
{
	m_neighbours.push_back(neighbour);
}

void Planet::setOwner(Player::Ptr owner) 
{
	m_owner = owner;
}

int Planet::getRubin() 
{
	return m_rubin;
}

int Planet::getMines() 
{
	return m_mines;
}

void Planet::addMines(int mines) 
{
	m_mines += mines;
}

std::list<Planet::Ptr> Planet::getNeighbours()
{
	return m_neighbours;
}

int Planet::getPosX()
{
	return m_posx;
}

int Planet::getPosY()
{
	return m_posy;
}

std::string Planet::getName()
{
    return m_name;

}

}