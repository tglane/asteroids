#include "DataModel.hpp"
#include <iostream>
#include <fstream>

namespace asteroids{

DataModel::DataModel(std::string filename) : m_planets()
{
    // player which runs this programm
    m_self = new Player();

    // enemy/ies that run the programm on other devices
    // information from network is needed
    m_enemy = new Player();
    getUniverse(filename);
}

void DataModel::getUniverse(std::string filename)
{

    std::ifstream f;
    f.open(filename);

    if(f.is_open())
    {
        int numvertex, posx, posy, mines;
        std::string name;

        f >> numvertex;

        // initialize all planets and add them to the map
        for(int i = 0; i < numvertex; i++)
        {
            f >> name >> posx >> posy >> mines;
            Planet* p = new Planet(name, posx, posy);

            m_planets[i] = p;
        }

        // add the edges to the map
        int from, to;
        while(!f.eof())
        {
            f >> from >> to;
            m_planets.at(from)->addNeighbour(m_planets.at(to));
            m_planets.at(to)->addNeighbour(m_planets.at(from));
        }

        f.close();
    }
}

std::map<int, Planet*> DataModel::getPlanets()
{
    return m_planets;
}

void DataModel::endOfRound()
{
    // TODO Update players ressources, money, ships, planets, mines

    // TODO make a json-data-package from the data and send it to the server
    //      listen for the response, start fights or next round
}

DataModel::~DataModel()
{
    delete m_self;
    delete m_enemy;
}

}