#include "DataModel.hpp"
#include <iostream>
#include <fstream>
#include <utility>

namespace asteroids{

DataModel::DataModel(std::string filename) : m_players(), m_planets(), m_edges()
{
    // player which runs this programm
    m_self = Player::Ptr(new Player(1,3000,0));

    // enemy/ies that run the programm on other devices
    // information from network is needed
    m_enemy = Player::Ptr(new Player());

    // when networking issues are solved the map is loaded later
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
            Planet::Ptr p = Planet::Ptr(new Planet(name, posx, posy, mines));

            m_planets[i] = p;
            m_nameToPlanets[name] = p;
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

    calculateFinance(getSelfPlayer());
    // TODO Update players ressources, money, ships, planets, mines

    // TODO make a json-data-package from the data and send it to the server
    //      listen for the response, start fights or next round

    // return if network response was succesful
    return true;
}


/*Code von Kay Bauer*/
bool DataModel::buyShip(Planet::Ptr selectedPlanet, Player::Ptr m_self)
{
    /*test druck*/
    std::cout << "Test für buyShip" << std::endl;
    std::cout << m_self->getRubin() << std::endl;
    /*test druck ende*/

    int Player_Rubin_Number = m_self->getRubin();
    if(Player_Rubin_Number >= Shipcost)
    {
        m_self->delRubin(Shipcost);
        /*test druck*/
        std::cout << m_self->getRubin() << std::endl;
        /*test druck ende*/
        std::shared_ptr<ShipOrder> NewShip = std::shared_ptr<ShipOrder>(new ShipOrder(selectedPlanet));
        m_self->putListShipOrder(NewShip);


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
        from->delShips(numShips);
		return true;
	}

	else {

		std::cout << "MoveOrder not successful"<< std::endl;
		return false;
	}


}
bool DataModel::buyMine(Planet::Ptr selectedPlanet, Player::Ptr m_self)
{
    /*test druck*/
    std::cout << "Test für buyMine" << std::endl;
    std::cout << selectedPlanet->getMinesBuild() << std::endl;
    std::cout << selectedPlanet->getMines() << std::endl;
    /*test druck ende*/
    if(selectedPlanet->getMinesHidden() < selectedPlanet->getMines())
    {
        int Player_Rubin_Number = m_self->getRubin();
        if(Player_Rubin_Number >= Minecost)
        {
            m_self->delRubin(Minecost);
            selectedPlanet->setMinesHidden();
             /*test druck*/
            std::cout << m_self->getRubin() << std::endl;
            /*test druck ende*/
            std::shared_ptr<MineOrder> NewMine = std::shared_ptr<MineOrder>(new MineOrder(selectedPlanet));
            m_self->putListMineOrder(NewMine); 
            return true;
        }

        return false;

    }

    return false;

}

void DataModel::TransaktionMine()
{
    std::list<std::shared_ptr<MineOrder>> m_TransaktionMine = m_self->getListMineOrder();

    for(std::list<std::shared_ptr<MineOrder>>::iterator it = m_TransaktionMine.begin(); it != m_TransaktionMine.end(); ++it)
    {
        std::shared_ptr<MineOrder> NewOrder = *it;

        Planet::Ptr NewShipToPlanet = NewOrder->getPlanet();

        NewShipToPlanet->setMinesBuild();
    }

}

void DataModel::TransaktionShip()
{
    std::list<std::shared_ptr<ShipOrder>> m_TransaktionShip = m_self->getListShipOrder();

    for(std::list<std::shared_ptr<ShipOrder>>::iterator it = m_TransaktionShip.begin(); it != m_TransaktionShip.end(); ++it)
    {
        std::shared_ptr<ShipOrder> NewOrder = *it;

        Planet::Ptr NewShipToPlanet = NewOrder->getPlanet();

        NewShipToPlanet->addShips(1);
    }


}

void DataModel::clearOrderList()
{
    m_self->ClearOrderListInPlayer();
}

Planet::Ptr DataModel::getPlanetFromId(int ID)
{
    return m_planets.at(ID);
}

Planet::Ptr DataModel::getPlanetFromName(std::string name)
{
    return m_nameToPlanets[name];
}

void DataModel::calculateFinance(Player::Ptr Player)
{
    std::list<std::shared_ptr<Planet>> m_planetsForGain = m_self->getListOfPLanets();
    int MineNumbers = 0;
    int MineGainWithNumbers = 0;
    for(std::list<std::shared_ptr<Planet>>::iterator it = m_planetsForGain.begin(); it != m_planetsForGain.end(); ++it)
    {
        Planet::Ptr PlanetFromPlayer = *it;

        MineNumbers += PlanetFromPlayer->getMinesBuild();
        std::cout <<"Test MineNumbers"<< std::endl;
        std::cout << MineNumbers << std::endl;
    }
    MineGainWithNumbers = MineNumbers * Minegain;
    m_self->addRubin(MineGainWithNumbers);
    TransaktionMine();
    TransaktionShip();
    clearOrderList();

}
  
void DataModel::startGame()
{

}

void DataModel::setStartPlanet(std::shared_ptr<Planet> startplanet)
{
	startplanet->setOwner(m_self);
	startplanet->addShips(1);
	m_self->addPlanet(startplanet);
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

void DataModel::updateAll(QJsonDocument update) {

}

Player::Ptr DataModel::getSelfPlayer()
{
    return m_self;
}

Player::Ptr DataModel::getEnemyPlayer(int id)
{
    return m_enemy;
}

void DataModel::findBattles()
{
    std::map<int, Planet::Ptr>::iterator it;
    for(it = m_planets.begin(); it != m_planets.end(); it++)
    {
        Planet::Ptr p = it->second;
        if(p->getInvader() != NULL)
        {
            // In case the defender ships are not present anymore
            if(p->getShips() == 0)
            {
                p->setOwner(p->getInvader());
                p->addShips(p->getInvaderShips());

            }
            else
            {
                //everything for battle is ready
                m_battles.push_back(std::shared_ptr<Battle>(new Battle(p, p->getOwner(), 
                                p->getInvader(), p->getShips(),
                                p->getInvaderShips())));
            }
            p->setInvader(NULL);
        }
    }
}

QJsonDocument DataModel::createJson(Player::Ptr player)
{
    // main QJson object in the document
    QJsonObject main;

    //insert id of the player
    main.insert("ID", player->getIdentity());
    main.insert("Name",  QString::fromStdString(player->getPlayerName()));
    main.insert("Rubin", player->getIdentity());

    QJsonArray planeets;

    //planeets.push_back();
    std::string planetString("Planet");
    /**
    for(uint i = 0; i < player->getPlanets().size(); i++)
    {
        std::string planetNumber = planetString + std::to_string(i);
        QString qPlanetNumber = QString::fromStdString(planetNumber);

        
    }
    */
   int i = 1;
   std::list<std::shared_ptr<Planet>> planetos = player->getPlanets();
    for(std::list<std::shared_ptr<Planet>>::iterator it = planetos.begin(); it != planetos.end(); ++it)
    {
        Planet::Ptr planet = *it;
        QJsonObject qPlanet;

        std::string planetNumber = planetString + std::to_string(i);
        QString qPlanetNumber = QString::fromStdString(planetNumber);

        qPlanet.insert("ID", getIDFromPlanet(planet));
        qPlanet.insert("Mines", planet->getMines());
        qPlanet.insert("Ships", planet->getShips());

        planeets.push_back(qPlanet);

        i++;

    }

    main.insert("PlanetArray", planeets);

    QJsonDocument theDocument(main);

    std::cout << theDocument.toJson().toStdString() << std::endl;

    return QJsonDocument();
}


void DataModel::performMovements()
{
    //get own list of moveorders
    std::list<std::shared_ptr<MoveOrder>> myMoveOrder = m_self->getListMoveOrder();
    //perform all moveorders
    for (std::list<std::shared_ptr<MoveOrder>>::iterator it = myMoveOrder.begin(); it != myMoveOrder.end(); ++it){
        //get moveorder
        std::shared_ptr<MoveOrder> moveOrder = *it;
        std::shared_ptr<Planet> origin = moveOrder->getOrigin();
        std::shared_ptr<Planet> destination = moveOrder->getDestination();
        int ships = moveOrder->getNumberShips();
        //take ships from origin planet

        if(destination->getOwner() == m_self){
            //the destination planet is of the same owner
            destination->addShips(ships);
        }else if(destination->getShips()==0){
            //the destination planet is of another player but he cannot defend, planet acquired
            destination->addShips(ships);
            destination->setOwner(m_self);
            destination->setInvader(NULL);
            destination->setInvaderShips(0);
        }else{
            //the destination planet is of another player but he might defend
            destination->setInvader(m_self);
            destination->addInvaderShips(ships);
        }
    }
}

int DataModel::getIDFromPlanet(Planet::Ptr planet)
{
    for(int i = 0; i < m_planets.size(); i++)
    {
        Planet::Ptr mapPlanet = m_planets.find(i)->second;
        if(mapPlanet->getName() == planet->getName())
        {
            return i;
        }
    }
}

DataModel::~DataModel()
{

}

}
