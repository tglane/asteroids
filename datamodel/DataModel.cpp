#include "datamodel/DataModel.hpp"


namespace asteroids{


DataModel::DataModel() : DataModel_Server()
{}

DataModel::DataModel(std::string filename) : DataModel_Server(filename)
{}

DataModel::DataModel(std::string filename, int id, std::string player_name ) : 
    DataModel_Server(filename, id, player_name)
{
    m_self = Player::Ptr(new Player(id, 3000, 0, player_name));
    addPlayer(m_self);
    getUniverse(filename);
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
    if(Id == MAIN2D || Id == MAIN3D)
    {
        m_mainWindow->showFullScreen();
        emit updateInfo();
    }
    if(Id == MAIN3D)
    {
        emit ((strategy::GameWindow*)m_mainWindow)->pause();
        //((MainWindow*)m_widgets[Id])->activate(true);
    }
    if(Id == MAIN2D || Id == SWITCH || Id == START)
    {
        emit ((strategy::GameWindow*)m_mainWindow)->play();
    }
    ((strategy::GameWindow*)m_mainWindow)->content()->setCurrentWidget(m_widgets[Id]);  
}

DataModel::~DataModel(){}

}
