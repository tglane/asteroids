#ifndef __DATAMODEL__HPP__
#define __DATAMODEL__HPP__

#include "Planet.hpp"
#include "Player.hpp"

#include <map>

using std::map;


/**
 * NOTE Maybe this class should also provide the servers technology
 *      like accepting incoming data packets and send back the updated data to the clients
 * 
 * OR   This data model runs on every client and the server, the clients data is updated
 *      by the server
 */


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

    /**
     * @brief   Frees the memory
     */
    ~DataModel();

    // many getter, setter/update methods for all the games data
    std::map<int, Planet*> getPlanets();


    /**
     * @brief   Called when a strategy round ends
     *          Updates all values and send them to the server
     */
    bool endOfRound();

    /*Code von Kay Bauer*/
    bool buyShip(Planet* selectedPlanet, Player* Player1);

    Planet* getPlanetFromId(int ID);

    std::list<std::pair<int,int>> getEdges();

private:
    /*Variablen von Kay*/
    int Player_Rubin_Number;

    int Shipcost = 500;
    /**
     * @brief   Loads all the planets from the given file
     */
    void getUniverse(std::string filename);

    // Map to hold all planets, filled by getUniverse()
    std::map<int, Planet*>  m_planets;

    std::list<std::pair<int, int>> m_edges;

    // The host Player
    Player*  m_self;

    // The client Player
    Player*  m_enemy;

};

}


#endif