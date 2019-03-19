#include "DataModel.hpp"

namespace asteroids{

DataModel::DataModel(std::string filename)
{
    // TODO initialize Players

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

}