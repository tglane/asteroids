#include "DataModel.hpp"
#include <iostream>
#include <fstream>
#include <utility>

namespace asteroids{

DataModel::DataModel(std::string filename) : m_planets(), m_edges()
{
    // player which runs this programm
    m_self = Player::Ptr(new Player());

    // enemy/ies that run the programm on other devices
    // information from network is needed
    m_enemy = Player::Ptr(new Player());
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
            Planet::Ptr p = Planet::Ptr(new Planet(name, posx, posy));

            m_planets[i] = p;
        }

        // add the edges to the map
        int from, to;
        while(!f.eof())
        {
            f >> from >> to;
            from--;
            to--;
            m_edges.push_back(std::make_pair(from, to));
            m_planets.at(from)->addNeighbour(m_planets.at(to));
            m_planets.at(to)->addNeighbour(m_planets.at(from));
        }

        f.close();
    }
}

std::map<int, Planet::Ptr> DataModel::getPlanets()
{
    return m_planets;
}

std::list<std::pair<int,int>> DataModel::getEdges()
{
    return m_edges;
}

bool DataModel::endOfRound()
{
    std::cout << "End of Round!" << std::endl;
    // TODO Update players ressources, money, ships, planets, mines

    // TODO make a json-data-package from the data and send it to the server
    //      listen for the response, start fights or next round

    // return if network response was succesful
    return true;
}
/*Code von Kay Bauer*/
bool DataModel::buyShip(Planet::Ptr selectedPlanet, Player::Ptr Player1)
{
    /*test druck*/
    std::cout << "Test für buyShip" << std::endl;
    std::cout << Player1->getRubin() << std::endl;
    /*test druck ende*/

    int Player_Rubin_Number = Player1->getRubin();
    if(Player_Rubin_Number >= Shipcost)
    {
        Player1->delRubin(Shipcost);
        /*test druck*/
        std::cout << Player1->getRubin() << std::endl;
        /*test druck ende*/

        return true;
    }
    
    return false;

}

bool DataModel::buyMine(Planet::Ptr selectedPlanet, Player::Ptr Player1)
{
    /*test druck*/
    std::cout << "Test für buyMine" << std::endl;
    std::cout << selectedPlanet->getMines() << std::endl;
    /*test druck ende*/
    if(selectedPlanet->getMines() == 0)
    {
        int Player_Rubin_Number = Player1->getRubin();
        if(Player_Rubin_Number >= Minecost)
        {
            Player1->delRubin(Minecost);
             /*test druck*/
            std::cout << Player1->getRubin() << std::endl;
            /*test druck ende*/

            return true;
        }

        return false;

    }

    return false;

}

Planet::Ptr DataModel::getPlanetFromId(int ID)
{
    return m_planets.at(ID);
}

void DataModel::startGame()
{


}

void DataModel::addWindow(int Id, QMainWindow* Window)
{
    m_Window[Id] = Window;

}

void DataModel::switchWindow(int Id)
{
    QMainWindow* Active = m_Window[Id];
    
}

DataModel::~DataModel()
{
    /*delete m_self;
    delete m_enemy;*/
}

}
