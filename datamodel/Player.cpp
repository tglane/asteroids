/*
 * Player.cpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 */

#include "Player.hpp"
#include <list>

namespace asteroids {

Player::Player() : m_planets()
{

}


Player::Player(int identity, int rubin, int ships, std::string player_name) : m_planets()
{
	m_identity = identity;
	m_rubin = rubin;
	m_ships = ships;
	m_name = player_name;
}

void Player::setPlanetsList(std::list<std::shared_ptr<Planet>> planets)
{
	m_planets = planets;
}

void Player::addPlanet(std::shared_ptr<Planet> planet)
{
	m_planets.push_back(planet);
}

int Player::getShips()
{
	return m_ships;
}

void Player::delShips(int ShipNumber)
{
	if(m_ships > 0)
	{
		if(m_ships >= ShipNumber)
		{
			m_ships -= ShipNumber;
		}
	}
}

std::list<Planet::Ptr> Player::getPlanets()
{
	return m_planets;
}

void Player::setShips(int ships)
{
	m_ships = ships;
}

void Player::setMines(int mines)
{
	m_mines = mines;
}

void Player::setIdentity(int id){
	m_identity = id;
}

int Player::getIdentity()
{
	return m_identity;
}

int Player::getRubin()
{
	return m_rubin;
}

void Player::addRubin(int add)
{
	m_rubin += add;
}

void Player::incShips(int Ships)
{
	std::cout << "Schiffe werden erhöht um 1" << std::endl;
	m_ships += Ships;
}

void Player::incMines()
{
	m_mines++;
}

int Player::getMines()
{
	return m_mines;
}

bool Player::delRubin(int del)
{
	if (del > m_rubin) {
		return false;
	}
	else
	{
		m_rubin -= del;
		return true;
	}
}

void Player::setPlayerName(std::string name)
{
	m_name = name;
}

std::string Player::getPlayerName()
{
	return m_name;
}

std::list<std::shared_ptr<ShipOrder>> Player::getListShipOrder()
{
	return m_shipOrders;
}

void Player::putListShipOrder(std::shared_ptr<ShipOrder> NewShipOrder)
{
	m_shipOrders.push_back(NewShipOrder);
}

std::list<std::shared_ptr<MineOrder>> Player::getListMineOrder()
{
	return m_mineOrders;
}

void Player::putListMineOrder(std::shared_ptr<MineOrder> NewMineOrder)
{
	m_mineOrders.push_back(NewMineOrder);
}

std::list<std::shared_ptr<MoveOrder>> Player::getListMoveOrder()
{
	return m_moveOrders;
}

void Player::putListMoveOrder(std::shared_ptr<MoveOrder> newMoveOrder)
{
	m_moveOrders.push_back(newMoveOrder);
}

void Player::ClearOrderListInPlayer()
{
	m_shipOrders.clear();
	m_mineOrders.clear();
	m_moveOrders.clear();
}

std::list<std::shared_ptr<Planet>> Player::getListOfPLanets()
{
	return m_planets;

}

void Player::RemovePlaneteFromList(Planet::Ptr Planet)
{
	std::list<Planet::Ptr>::iterator it;
	int Postion = 0;
	std::list<Planet::Ptr>::iterator PostionRemove = m_planets.begin();
    for(it = m_planets.begin(); it != m_planets.end(); )
	{
		Planet::Ptr PlanetsFromList = *it;
		std::cout << Postion <<std::endl;
		if(Planet == PlanetsFromList)
		{
			std::advance(PostionRemove, Postion);
			std::cout << "test In Remove List before" <<std::endl;
			it = m_planets.erase(PostionRemove);
			std::cout << "test In Remove List after" <<std::endl;
		}
		else
		{
		it++;
		Postion++;
		}




	}

}

void Player::PrintPlanetsList()
{
	std::cout << m_name << std::endl;
	std::list<Planet::Ptr>::iterator it;
    for(it = m_planets.begin(); it != m_planets.end(); it++)
	{
		Planet::Ptr PlanetsFromList = *it;
		std::cout << PlanetsFromList->getName() <<std::endl;
	}

}


/*void Player::updateResources() {

	std::list<std::shared_ptr<Planet>>::iterator i;
	m_rubin = 0;
	m_ships = 0;
	for( i = m_planets.begin(); i != m_planets.end(); i++)
	{
		std::shared_ptr<Planet> planet = *i;
		m_rubin += planet->getRubin();
		m_ships += planet->getInvaderShips();

	}
}*/

}
