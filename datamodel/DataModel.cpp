#include "datamodel/DataModel.hpp"
#include "view2D/GameWindow.hpp"
#include "view2D/MainWindow2D.hpp"

namespace asteroids{


DataModel::DataModel() : DataModel_Server()
{}

DataModel::DataModel(std::string filename) : DataModel_Server(filename)
{}

DataModel::DataModel(std::string filename, int id, std::string player_name ) : 
    DataModel_Server(filename, id, player_name)
{
    m_self = Player::Ptr(new Player(id, 3000, 1, player_name));
    addPlayer(m_self);
    getUniverse(filename);
}


void DataModel::getUniverse(std::string filename)
{
    std::ifstream f;
    f.open(filename);

    if(f.is_open())
    {
        int numvertex, posx, posy, mines;
        std::string name;

        f >> numvertex >> posx >> posy;
        ((strategy::MainWindow2D*)m_widgets[MAIN2D])->setMapSize(posx, posy);

        f >> Shipcost >> Minecost >> Shipyardcost >> Minegain;

        // initialize all planets and add them to the map
        for(int i = 0; i < numvertex; i++)
        {
            f >> name >> posx >> posy >> mines;
            Planet::Ptr p = Planet::Ptr(new Planet(name, posx, posy, mines));

            std::cout << "adding planet: " << name << " " << i << std::endl;
            m_planets[i] = p;
            m_nameToPlanets[name] = p;
            m_planetNameToId[name] = i;
        }

        // add the edges to the map
        int from, to;
        while(!f.eof())
        {
            f >> from >> to;
            from--;
            to--;
            m_edges.push_back(std::make_pair(from, to));
            m_planets.at(from)->addNeighbour(m_planets.at(to));
            m_planets.at(to)->addNeighbour(m_planets.at(from));
        }

        f.close();
    }
    else
    {
        std::cout << "failed to open map file" << std::endl;
    }
}

void DataModel::addMainWindow(QMainWindow* window)
{
    m_mainWindow = window;
}

void DataModel::addWidget(int Id, QWidget* widget)
{
    m_widgets.insert(std::pair<int, QWidget*>(Id, widget));
}


void DataModel::switchWindow(int Id)
{
    if(Id == MAIN2D)
    {
        ((strategy::GameWindow*)m_mainWindow)->content()->setCurrentWidget(m_widgets[Id]);
        //((strategy::MainWindow2D*)m_widgets[MAIN2D])->resizeEvent(NULL);
        emit updateInfo();
        //flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

        //code for showing window
        m_mainWindow->showFullScreen();
        m_mainWindow->raise();
        //m_mainWindow->requestActivate();
        // m_mainWindow->showFullScreen();
    }
    if(Id == MAIN3D)
    {
        emit ((strategy::GameWindow*)m_mainWindow)->pause();
        m_mainWindow->hide();
        //((MainWindow*)m_widgets[Id])->activate(true);
    }
    if(Id == END)
    {
        emit endOfGame();
    }
    if(Id == MAIN2D || Id == SWITCH || Id == START || Id == END)
    {
        ((strategy::GameWindow*)m_mainWindow)->content()->setCurrentWidget(m_widgets[Id]);
        emit ((strategy::GameWindow*)m_mainWindow)->play();
        m_mainWindow->raise();
    }
}

bool DataModel::WinCondition()
{
    if(first_round)
    {
        first_round = false;
        return false;
    }
    else
    {
        std::map<int, Planet::Ptr>::iterator it;
        int NumberOfPlanets = m_planets.size();
        int selfOwnedPlanets = 0;
        int enemyOwnedPlanets = 0;
        std::cout << "Anzahl der Planeten" << std::endl;
        std::cout << NumberOfPlanets << std::endl;
        for(it = m_planets.begin(); it != m_planets.end(); it++)
        {
            Planet::Ptr Planets = it->second;
            if(Planets->getOwner() == m_self)
            {
                selfOwnedPlanets++;
            }
            else if(Planets->getOwner() == m_enemy)
            {
                enemyOwnedPlanets++;
            }
        }

        if(NumberOfPlanets == selfOwnedPlanets)
        {
            result = 1;
            //emit endOfGame();
            switchWindow(END);
            std::cout << "Gewonnen" <<std::endl;
            return true;
        }
        if(selfOwnedPlanets == 0)
        {
            result = 2;
            //emit endOfGame();
            switchWindow(END);
            std::cout << "Verloren" << std::endl;
            return true;
        }
        if(NumberOfPlanets == enemyOwnedPlanets)
        {
            result = 2;
            //emit endOfGame();
            switchWindow(END);
            std::cout << "Verloren" <<std::endl;
            return true;
        }
        if(enemyOwnedPlanets == 0)
        {
            result = 1;
            //emit endOfGame();
            switchWindow(END);
            std::cout << "Gewonnen" << std::endl;
            return true;
        }
        return false;
    }
}

DataModel::~DataModel(){}

}
