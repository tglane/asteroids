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

Planet::Planet(const std::string name, int rubin = 0; int mines = 0) : m_neighbours(), m_owner() {

	m_name = name;
	m_rubin = rubin;
	m_mines = mines;

}

int Planet::getShips() {
	return m_ships;
}

void Planet::addShips(int ships) {
	m_ships += ships;
}

boolean Planet::delShips(int ships) {

	if (ships > m_ships) return false;

		else m_ships -= ships; return true;
}

void Planet::addNeighbour(Planet neighbour) {

	m_neighbours.push_back(neighbour);
}

void Planet::setOwner(Player owner) {

	m_owner = owner;
}

int Planet::getRubin() {

	return m_rubin;
}

int Planet::getMines() {

	return m_mines;
}

void Planet::addMines(int mines) {
	m_mines += mines;
}

std::list<Planet> Planet::getNeighbours() {
	return m_neighbours;
}

}



