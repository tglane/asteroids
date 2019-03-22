#include "DataModel.hpp"
#include <iostream>
#include <fstream>
#include <utility>

namespace asteroids{

DataModel::DataModel(std::string filename) : m_planets(), m_edges()
{
    // player which runs this programm
    m_self = Player::Ptr(new Player());

    // enemy/ies that run the programm on other devices
    // information from network is needed
    m_enemy = Player::Ptr(new Player());
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

        f >> numvertex;

        // initialize all planets and add them to the map
        for(int i = 0; i < numvertex; i++)
        {
            f >> name >> posx >> posy >> mines;
            Planet::Ptr p = Planet::Ptr(new Planet(name, posx, posy));

            m_planets[i] = p;
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
}

std::map<int, Planet::Ptr> DataModel::getPlanets()
{
    return m_planets;
}

std::list<std::pair<int,int>> DataModel::getEdges()
{
    return m_edges;
}

bool DataModel::endOfRound()
{
    std::cout << "End of Round!" << std::endl;
    // TODO Update players ressources, money, ships, planets, mines

    // TODO make a json-data-package from the data and send it to the server
    //      listen for the response, start fights or next round

    // return if network response was succesful
    return true;
}


/*Code von Kay Bauer*/
bool DataModel::buyShip(Planet::Ptr selectedPlanet, Player::Ptr Player1)
{
    /*test druck*/
    std::cout << "Test für buyShip" << std::endl;
    std::cout << Player1->getRubin() << std::endl;
    /*test druck ende*/

    int Player_Rubin_Number = Player1->getRubin();
    if(Player_Rubin_Number >= Shipcost)
    {
        Player1->delRubin(Shipcost);
        /*test druck*/
        std::cout << Player1->getRubin() << std::endl;
        /*test druck ende*/
        std::shared_ptr<ShipOrder> NewShip = std::shared_ptr<ShipOrder>(new ShipOrder(selectedPlanet));
        Player1->putListShipOrder(NewShip);


        return true;
    }
    
    return false;

}
//TODO: MoveOrder in entsprechende Liste
bool DataModel::moveShips(Planet::Ptr from, Planet::Ptr to, int numShips) {

	std::cout << "MoveOrder " << numShips << " Ships from Planet " << from->getName() << " to Planet " << to->getName() << std::endl;

	if(from->getShips() >= numShips)
	{
		MoveOrder::Ptr move = MoveOrder::Ptr(new MoveOrder(from, to, numShips));
		m_self->putListMoveOrder(move);
		std::cout << "MoveOrder successful"<< std::endl;

		return true;
	}

	else {

		std::cout << "MoveOrder not successful"<< std::endl;
		return false;
	}


}
bool DataModel::buyMine(Planet::Ptr selectedPlanet, Player::Ptr Player1)
{
    /*test druck*/
    std::cout << "Test für buyMine" << std::endl;
    std::cout << selectedPlanet->getMines() << std::endl;
    /*test druck ende*/
    if(selectedPlanet->getMines() == 0)
    {
        int Player_Rubin_Number = Player1->getRubin();
        if(Player_Rubin_Number >= Minecost)
        {
            Player1->delRubin(Minecost);
             /*test druck*/
            std::cout << Player1->getRubin() << std::endl;
            /*test druck ende*/
            std::shared_ptr<MineOrder> NewMine = std::shared_ptr<MineOrder>(new MineOrder(selectedPlanet));
            Player1->putListMineOrder(NewMine); 
            return true;
        }

        return false;

    }

    return false;

}

void DataModel::TransaktionMine(Player::Ptr Player1)
{
    std::list<std::shared_ptr<MineOrder>> m_TransaktionMine = Player1->getListMineOrder();

    for(std::list<std::shared_ptr<MineOrder>>::iterator it = m_TransaktionMine.begin(); it != m_TransaktionMine.end(); ++it)
    {
        std::shared_ptr<MineOrder> NewOrder = *it;

        Planet::Ptr NewShipToPlanet = NewOrder->getPlanet();

        NewShipToPlanet->addMines(1);
    }

}

void DataModel::TransaktionShip(Player::Ptr Player1)
{
    std::list<std::shared_ptr<ShipOrder>> m_TransaktionShip = Player1->getListShipOrder();

    for(std::list<std::shared_ptr<ShipOrder>>::iterator it = m_TransaktionShip.begin(); it != m_TransaktionShip.end(); ++it)
    {
        std::shared_ptr<ShipOrder> NewOrder = *it;

        Planet::Ptr NewShipToPlanet = NewOrder->getPlanet();

        NewShipToPlanet->addShips(1);
    }


}

void DataModel::clearOrderList(Player::Ptr Player1)
{
    Player1->ClearOrderListInPlayer();
}

Planet::Ptr DataModel::getPlanetFromId(int ID)
{
    return m_planets.at(ID);
}

  
void DataModel::startGame()
{

}

void DataModel::setStartPlanet(std::shared_ptr<Planet> startplanet)
{
	startplanet->setOwner(m_self);
	startplanet->addShips(1);
	m_self->addPlanet(startplanet);
	startplanet->addShips(1);
}

void DataModel::addWindow(int Id, QMainWindow* Window)
{
    m_Window[Id] = Window;
}

void DataModel::switchWindow(int Id)
{
    QMainWindow* Active = m_Window[Id];
    Active->showFullScreen();  
}

Player::Ptr DataModel::getSelfPlayer()
{
    return m_self;
}

Player::Ptr DataModel::getEnemyPlayer()
{
    return m_enemy;
}

void DataModel::findBattles()
{
    std::map<int, Planet::Ptr>::iterator it;
    for(it = m_planets.begin(); it != m_planets.end(); it++)
    {
        if(it->second->getInvader() != NULL)
        {
            m_battles.push_back(std::shared_ptr<Battle>(new Battle(it->second, it->second->getOwner(), 
                            it->second->getInvader(), it->second->getShips(),
                            it->second->getInvaderShips())));
        }
    }
}

DataModel::~DataModel()
{

}

}
