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
#include "Battle.hpp"

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

    enum { START, MAIN2D, MAIN3D };
    using Ptr = std::shared_ptr<DataModel>;

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

    /*Kauf Methoden start*/
    bool buyShip(Planet::Ptr selectedPlanet);

    bool buyMine(Planet::Ptr selectedPlanet);

    void TransaktionShip();

    void TransaktionMine();

    void clearOrderList();

    /*Kauf Methoden ende*/
    bool moveShips(Planet::Ptr from, Planet::Ptr to, int numShips);

    Planet::Ptr getPlanetFromId(int ID);

    std::list<std::pair<int,int>> getEdges();

    void calculateFinance();
    
    void startGame();
    /**
     * @brief sets the choosen startplanet: 1 ship to the planet, m_self as new owner
     * @param shared ptr to choosen planet
     */
    void setStartPlanet(std::shared_ptr<Planet> startplanet);

    void addWindow(int Id, QMainWindow* Window);

    void switchWindow(int Id);

    Player::Ptr getSelfPlayer();

    Player::Ptr getEnemyPlayer(int id);

    /**
     * @brief   Finds occuring battles at the end of each round,
     *          fills list of battles
     */
    void findBattles();

private:

    int m_playerid;

    /*Variablen von Kay*/

    int Shipcost = 500;

    int Minecost = 1000;

    int Minegain = 750;
    /**
     * @brief   Loads all the planets from the given file
     */
    void getUniverse(std::string filename);

    std::map<int, Player::Ptr> m_players;

    // Map to hold all planets, filled by getUniverse()
    std::map<int, Planet::Ptr>  m_planets;

    std::list<std::pair<int, int>> m_edges;

    // The host Player
    Player::Ptr  m_self;

    // The client Player
    Player::Ptr  m_enemy;

    // Map of Windows
    std::map<int, QMainWindow*> m_Window;

    // List of upcoming battles
    std::list<std::shared_ptr<Battle>> m_battles;
};

}


#endif
