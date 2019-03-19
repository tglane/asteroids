#include "DataModel.hpp"

namespace asteroids{

DataModel::DataModel(std::string filename)
{
    // player which runs this programm
    m_self = new Player(...?);

    // enemy/ies that run the programm on other devices
    // information from network is needed
    m_enemy = new Player(...?);

    getUniverse(filename);
}

DataModel::getUniverse(std::string filename)
{
    m_planets = std::map();

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
            Planet* p = new Planet(name, posx, posy, mines);

            m_planets[i] = p;
        }

        // add the edges to the map
        int from, to;
        while(!f.eof())
        {
            f >> from >> to;
            m_planets[from].addNeighbour(m_planets[to]);
            m_planets[to].addNeighbour(m_planets[from]);
        }

        f.close();
    }
}

DataModel::getPlanets()
{
    return m_planets;
}

DataModel::endOfRound()
{
    // TODO Update players ressources, money, ships, planets, mines

    // TODO make a json-data-package from the data and send it to the server
    //      listen for the response, start fights or next round
}

}