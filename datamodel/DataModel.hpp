#ifndef __DATAMODEL__HPP__
#define __DATAMODEL__HPP__

#include "Planet.hpp"
#include "Player.hpp"

namespace asteroids{

class DataModel
{

public:
    /**
     * @brief   Initilizes a new data model
     * 
     * @param filename  the mapfile including all planets and their connections
     */
    DataModel(std::string filename);

    ~DataModel();

    // many getter, setter/update methods for all the games data

private:

    /**
     * @brief   Loads all the planets from the given file
     */
    void getUniverse(std::string filename);

    // Map to hold all planets, filled by getUniverse()
    std::map<std::string, Planet*>   m_planets;

    // The host Player
    Player*  host;

    // The client Player
    Player*  client;

};



}


#endif