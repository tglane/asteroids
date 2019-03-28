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

Planet::Planet(std::string name, int posx, int posy, int mines,/* int rubin*/ int rubinLeft) : m_neighbours()
{
	m_posx = posx;
	m_posy = posy;
	m_name = name;
	//m_rubin = rubin;
	m_mines = mines;
	m_minesbuild = 0;
	m_minesHidden = 0;
	m_ships = 0;
	m_owner = nullptr;
	m_invader = nullptr;
	m_invaderShips = 0;
	m_ships_ordered = 0;

	m_rubinLeft = rubinLeft;

	m_shipyardBuilt = false;

}


int Planet::getShips()
{
	return m_ships;
}

void Planet::addShips(int ships)
{
	m_ships += ships;
	m_owner->incShips(ships);
}

bool Planet::delShips(int ships)
{
	if (ships > m_ships)
	{
		return false;
	}
	else if(m_ships > 0)
	{
		m_ships -= ships;
		return true;
	}

	else
	{
		return false;

	}
}

void Planet::addNeighbour(Planet::Ptr neighbour)
{
	m_neighbours.push_back(neighbour);
}

void Planet::setOwner(std::shared_ptr<Player> owner)
{
	m_owner = owner;
}

void Planet::setInvader(std::shared_ptr<Player> invader)
{
	m_invader = invader;
}

void Planet::setInvaderShips(int invaderShips)
{
	 m_invaderShips = invaderShips;
}

void Planet::addInvaderShips(int invaderShips)
{
	m_invaderShips = m_invaderShips+invaderShips;
}

int Planet::getInvaderShips()
{
	return m_invaderShips;
}

std::shared_ptr<Player> Planet::getOwner()
{
	return m_owner;
}

std::shared_ptr<Player> Planet::getInvader()
{
	return m_invader;
}

void Planet::setMines(int mines)
{
	m_mines = mines;
}

void Planet::setShips(int ships)
{
	m_ships = ships;
}

/*int Planet::getRubin()
{
	return m_rubin;
}*/

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

int Planet::getMinesHidden()
{
	return m_minesHidden;

}

void Planet::setMinesHidden()
{
	m_minesHidden += 1;

}

int Planet::getMinesBuild()
{
	return m_minesbuild;

}

void Planet::setMinesBuild()
{
	m_minesbuild += 1;
	m_owner->incMines();
}

int Planet::getShipsOrdered()
{
	return m_ships_ordered;
}

void Planet::incShipsOrdered()
{
	m_ships_ordered++;
}

void Planet::resetShipsOrdered()
{
	m_ships_ordered = 0;
}

int Planet::subtractEarnings(int mineRubin)
{
	 int earnings = this->calculateEarnings(mineRubin);
	 m_rubinLeft -= earnings;
	 return earnings;
}

int Planet::getRubinLeft()
{
	return m_rubinLeft;
}

void Planet::setRubinLeft(int rubinLeft)
{
	m_rubinLeft = rubinLeft;
}

int Planet::calculateEarnings(int mineRubin)
{
	return mineRubin * m_mines;
}

bool Planet::getShipyardBuilt()
{
	return m_shipyardBuilt;
}

void Planet::buildShipyard()
{
	if(m_shipyardBuilt == true)
	{
		std::cerr << "Shipyard already has been built before" << std::endl;
	}
	m_shipyardBuilt = true;
}


}
