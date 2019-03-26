#ifndef __DATAMODEL__HPP__
#define __DATAMODEL__HPP__

#include "Planet.hpp"
#include "Player.hpp"
#include "MoveOrder.hpp"

#include <map>

#include <QMainWindow>

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QStackedWidget>


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

class DataModel : public QObject
{
    Q_OBJECT

public:

    enum { MAIN2D, MAIN3D, START, END };
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
    bool buyShip(Planet::Ptr selectedPlanet, Player::Ptr m_self);

    bool buyMine(Planet::Ptr selectedPlanet, Player::Ptr m_self);

    void TransaktionShip();

    void TransaktionMine();

    void clearOrderList();

    /*Kauf Methoden ende*/
    bool moveShips(Planet::Ptr from, Planet::Ptr to, int numShips);

    Planet::Ptr getPlanetFromId(int ID);

    Planet::Ptr getPlanetFromName(std::string name);

    int getIDFromPlanetName(std::string name);

    std::list<std::pair<int,int>> getEdges();

    void calculateFinance(Player::Ptr Player);
    
    void startGame();
    /**
     * @brief sets the choosen startplanet: 1 ship to the planet, m_self as new owner
     * @param shared ptr to choosen planet
     */
    void setStartPlanet(std::shared_ptr<Planet> startplanet);

    void addMainWindow(QStackedWidget* window);

    void addWidget(int Id, QWidget* widget);

    void switchWindow(int Id);

    Player::Ptr getSelfPlayer();

    Player::Ptr getEnemyPlayer(int id);

    bool updateAll(QJsonDocument &update); // @suppress("Type cannot be resolved")



    /**
     * @brief   Finds occuring battles at the end of each round,
     *          fills list of battles, and gives planets to invaders
     *          on planets that dont have defenders,
     */
    void findBattles();

    /**
     * @brief Creates Json File, which includes information about a certain player and his planets, 
     *        and his invasions
     * @param player The player for which the information should be sent
     * @return the created Json File
     */
    QJsonDocument createJson(Player::Ptr player);

    /**
     * OBSOLETE
     * @brief Creates Json File, which includes player identification, player rubin status, and orders
     * @param player The player for which the information should be sent
     * @return the created Json File
     
    QJsonDocument createJsonOrders(Player::Ptr player);
    */

    void performMovements(Player::Ptr player);

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

    int getResult() { return result; }

    /**
     * @brief adds player to map of players
     * @param Player to be added
     */
    void addPlayer(Player::Ptr player);

    Player::Ptr getPlayerByID(int i);

signals:
    void updateInfo();

    void initMap();


private:

    // 0 = not finished, 1 = victory, 2 = defeat
    int result = 0;

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

    std::map<std::string, Planet::Ptr> m_nameToPlanets;

    std::map<std::string, int> m_planetNameToId;

    std::list<std::pair<int, int>> m_edges;

    // The host Player
    Player::Ptr  m_self;

    // The client Player
    Player::Ptr  m_enemy;

    // Map of Windows
    std::map<int, QWidget*> m_widgets;

    QStackedWidget* m_mainWindow;

    // List of upcoming battles
    std::list<std::shared_ptr<Battle>> m_battles;
};

}


#endif
