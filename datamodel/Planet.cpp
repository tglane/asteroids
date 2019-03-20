/*
 * Planet.cpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 */


#include "Planet.hpp"
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

void Planet::addNeighbour(Planet* neighbour) 
{
	m_neighbours.push_back(neighbour);
}

void Planet::setOwner(Player* owner) 
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

std::list<Planet*> Planet::getNeighbours() 
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

}