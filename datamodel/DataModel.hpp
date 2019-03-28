#ifndef __DATAMODEL__HPP__
#define __DATAMODEL__HPP__

#include "datamodel/DataModel_Server.hpp"

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

class DataModel : public DataModel_Server
{

Q_OBJECT

public:
    using Ptr = std::shared_ptr<DataModel>;
    /**
     * @brief   Initilizes a new data model
     * 
     * @param filename  the mapfile including all planets and their connections
     */
    DataModel();

    DataModel(std::string filename);

    DataModel(std::string level, int id, std::string player_name );
    /**
     * @brief   Frees the memory
     */
    ~DataModel();


    void addMainWindow(QMainWindow* window);

    void addWidget(int Id, QWidget* widget);

    QWidget* getWidget(int Id) { return m_widgets[Id];};

    void getUniverse(std::string filename);

    void WinCondition();

    bool endOfRound();

public slots:

    void switchWindow(int Id);


private:

    // Map of Windows
    std::map<int, QWidget*> m_widgets;

    QMainWindow* m_mainWindow;
};

}


#endif
