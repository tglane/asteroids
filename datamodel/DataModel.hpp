#ifndef __DATAMODEL__HPP__
#define __DATAMODEL__HPP__

#include "Planet.hpp"
#include "Player.hpp"


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
    void endOfRound();

private:

    /**
     * @brief   Loads all the planets from the given file
     */
    void getUniverse(std::string filename);

    // Map to hold all planets, filled by getUniverse()
    std::map<int, Planet*>  m_planets;

    // The host Player
    Player*  m_self;

    // The client Player
    Player*  m_enemy;

};

}


#endif