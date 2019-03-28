#ifndef __DATAMODEL_SERVER_HPP__
#define __DATAMODEL_SERVER_HPP__


#include "Planet.hpp"
#include "Player.hpp"
#include "MoveOrder.hpp"

#include <map>

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>


#include "MoveOrder.hpp"
#include "MineOrder.hpp"
#include "ShipOrder.hpp"
#include "ShipyardOrder.hpp"
#include "Battle.hpp"

using std::map;



namespace asteroids{

class DataModel_Server : public QObject
{

Q_OBJECT

public:

    enum { MAIN2D, MAIN3D, START, END, SWITCH };
    using Ptr = std::shared_ptr<DataModel_Server>;

    /**
     * @brief   Initilizes a new data model
     * 
     * @param filename  the mapfile including all planets and their connections
     */
    DataModel_Server();

    DataModel_Server(std::string level, int id, std::string player_name );
    /**
     * @brief   Frees the memory
     */
    ~DataModel_Server();

    DataModel_Server(std::string filename);

    // many getter, setter/update methods for all the games data
    std::map<int, Planet::Ptr> getPlanets();



    /**
     * @brief   Called when a strategy round ends
     *          Updates all values and send them to the server
     */
    bool endOfRound();

    /*Kauf Methoden start*/
    bool buyShip(Planet::Ptr selectedPlanet, Player::Ptr m_self);

    bool buyMine(Planet::Ptr selectedPlanet, Player::Ptr m_self);

    bool buyShipyard(Planet::Ptr selectedPlanet, Player::Ptr m_self);

    void TransaktionShip();

    void TransaktionMine();

    void TransaktionShipyard();

    void clearOrderList();

    /*Kauf Methoden ende*/
    bool moveShips(Planet::Ptr from, Planet::Ptr to, int numShips);

    Planet::Ptr getPlanetFromId(int ID);

    Planet::Ptr getPlanetFromName(std::string name);

    int getIDFromPlanetName(std::string name);

    std::list<std::pair<int,int>> getEdges();
    /**
     * added: subtractEarnings for each planet
     */
    void calculateFinance(Player::Ptr Player);
    
    void startGame();
    /**
     * @brief sets the choosen startplanet: 1 ship to the planet, m_self as new owner
     * @param shared ptr to choosen planet
     */
    void setStartPlanet(std::shared_ptr<Planet> startplanet);

    Player::Ptr getSelfPlayer();

    Player::Ptr getEnemyPlayer();

    bool updateAll(QJsonObject &update); // @suppress("Type cannot be resolved")



    /**
     * @brief   Finds occuring battles at the end of each round,
     *          fills list of battles, and gives planets to invaders
     *          on planets that dont have defenders,
     */
    std::vector<Battle::Ptr> findBattles();

    /**
     * @brief Creates Json File, which includes information about a certain player and his planets, 
     *        and his invasions
     * @param player The player for which the information should be sent
     * @return the created Json File
     */
    QJsonObject createJson(Player::Ptr player);

    /**
     * @brief creates QJsonobject representation of given Battle
     * @return QJsonobject representation of given Battle
     */
    QJsonObject createBattleJson(Battle::Ptr battle);

    /**
     * OBSOLETE
     * @brief Creates Json File, which includes player identification, player rubin status, and orders
     * @param player The player for which the information should be sent
     * @return the created Json File
     
    QJsonDocument createJsonOrders(Player::Ptr player);
    */

    void performMovements(Player::Ptr player);

    void constructPlayer(int id, std::string player_name, bool is_self);

    void BattleReport();

    /**
     * Returns the id of a given planet
     * @param planet Planet for which the id should be returned
     * @return the id of the given planet
     */
    int getIDFromPlanet(Planet::Ptr planet);

    void WinCondition();

    void BattlePhase();

    int getShipCost() { return Shipcost; }

    int getMineCost() { return Minecost; }

    int getShipyardCost() {return Shipyardcost; }

    int getResult() { return result; }

    /**
     * @brief adds player to map of players
     * @param Player to be added
     */
    void addPlayer(Player::Ptr player);

    /**
     * @brief returns the Player which holds the given id
     * @param i The ID of the player to be found
     * @return the player which holds this id, if player with given is nonexistant 
     *         new player with id -1 is returned
     */
    Player::Ptr getPlayerByID(int i);

    void getUniverse(std::string filename);
    
    /**
     * @brief   Loads all the planets from the given file
     */
    void printPlanets();

signals:
    void updateInfo();
    void initMap();


protected:

    // 0 = not finished, 1 = victory, 2 = defeat
    int result = 0;

    int m_playerid;

    /*Variablen von Kay*/

    int Shipcost = 500;

    int Minecost = 1000;

    int Shipyardcost = 2000;

    int Minegain = 750;
    

    std::map<int, Player::Ptr> m_players;

    // Map to hold all planets, filled by getUniverse()
    std::map<int, Planet::Ptr>  m_planets;

    std::map<std::string, Planet::Ptr> m_nameToPlanets;

    std::map<std::string, int> m_planetNameToId;

    std::list<std::pair<int, int>> m_edges;

    // The host Player
    Player::Ptr  m_self;

    // The client Player
    Player::Ptr  m_enemy;

    // List of upcoming battles
    std::vector<std::shared_ptr<Battle>> m_battles;

};



}



#endif
