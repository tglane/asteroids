/*
 * Player.cpp
 *
 *  Created on: 19.03.2019
 *      Author: sbuesing
 */

#include "Player.hpp"
#include <list>

namespace asteroids {


Player::Player(const int identity, int rubin = 0, int ships = 0) : m_planets() {

	m_identitiy = identitiy;
	m_rubin = rubin;
	m_ships = ships;

}

int Player::getShips() {

	return m_ships;
}

std::list<Planet> Player::getPlanets() {

	return m_planets;

}

int Player::getIdentity() {

	return m_identity;
}

int Player::getRubin() {

	return m_rubin;
}

void Player::addRubin(int add) {

	m_rubin += add;
}

bool Player::delRubin(int del) {

	if (del > m_rubin) return false;
	else m_rubin -= del; return true;
}



}


