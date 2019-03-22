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

void Player::addPlanet(std::shared_ptr<Planet> planet)
{
	m_planets.push_back(planet);
}

int Player::getShips() 
{
	return m_ships;
}

std::list<Planet::Ptr> Player::getPlanets()
{
	return m_planets;
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
