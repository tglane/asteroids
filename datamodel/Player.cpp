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


Player::Player(int identity, int rubin, int ships) : m_planets()
{
	m_identity = identity;
	m_rubin = rubin;
	m_ships = ships;
	m_name = "Slave of the Universe";
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

}
