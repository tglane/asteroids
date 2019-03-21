#ifndef __DATAMODEL__HPP__
#define __DATAMODEL__HPP__

#include "Planet.hpp"
#include "Player.hpp"
#include "MoveOrder.hpp"

#include <map>
#include <QMainWindow>

#include "MoveOrder.hpp"
#include "MineOrder.hpp"
#include "ShipOrder.hpp"

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
    std::map<int, Planet::Ptr> getPlanets();



    /**
     * @brief   Called when a strategy round ends
     *          Updates all values and send them to the server
     */
    bool endOfRound();

    /*Code von Kay Bauer*/

    /*Kauf Methoden start*/
    bool buyShip(Planet::Ptr selectedPlanet, Player::Ptr Player1);

    bool buyMine(Planet::Ptr selectedPlanet, Player::Ptr Player1);

    void TransaktionShip(Player::Ptr Player1);

    void TransaktionMine(Player::Ptr Player1);

    void clearOrderList(Player::Ptr Player1);

    /*Kauf Methoden ende*/
    bool moveShips(Planet::Ptr from, Planet::Ptr to, int numShips);

    Planet::Ptr getPlanetFromId(int ID);

    std::list<std::pair<int,int>> getEdges();

    void startGame();

    void addWindow(int Id, QMainWindow* Window);

    void switchWindow(int Id);

private:
    /*Variablen von Kay*/

    int Shipcost = 500;

    int Minecost = 1000;
    /**
     * @brief   Loads all the planets from the given file
     */
    void getUniverse(std::string filename);

    // Map to hold all planets, filled by getUniverse()
    std::map<int, Planet::Ptr>  m_planets;

    std::list<std::pair<int, int>> m_edges;

    // The host Player
    Player::Ptr  m_self;

    // The client Player
    Player::Ptr  m_enemy;

    // Map of Windows
    std::map<int, QMainWindow*> m_Window;

};

}


#endif
