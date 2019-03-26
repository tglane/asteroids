#include "DataModel.hpp"
#include "view/MainWindow.hpp"
#include <iostream>
#include <fstream>
#include <utility>



namespace asteroids{

DataModel::DataModel(std::string filename) : m_players(), m_planets(), m_edges()
{
    // player which runs this programm
    m_self = Player::Ptr(new Player(1,3000,0));
    addPlayer(m_self);

    // enemy/ies that run the programm on other devices
    // information from network is needed
    //m_enemy = Player::Ptr(new Player());
    

    // when networking issues are solved the map is loaded later
    getUniverse(filename);

    Planet::Ptr Test = getPlanetFromId(5);
 	Planet::Ptr Test2 = getPlanetFromId(6);
	Planet::Ptr Test3 = getPlanetFromId(7);
	m_enemy = Player::Ptr(new Player(2,3000,0));
	
    Test->setOwner(m_enemy);
    Test2->setOwner(m_enemy);
    Test3->setOwner(m_enemy);
    Test3->addShips(3);
	m_enemy->addPlanet(Test);
	m_enemy->addPlanet(Test2);
	m_enemy->addPlanet(Test3);
    addPlayer(m_enemy);
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

    performMovements(getSelfPlayer());
    findBattles();
    BattlePhase();
    m_self->PrintPlanetsList();
    m_enemy->PrintPlanetsList();
    BattleReport();
    WinCondition();
    m_self->PrintPlanetsList();
    m_enemy->PrintPlanetsList();


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

bool DataModel::moveShips(Planet::Ptr from, Planet::Ptr to, int numShips) {

	std::cout << "MoveOrder " << numShips << " Ships from Planet " << from->getName() << " to Planet " << to->getName() << std::endl;

	if(from->getShips() >= numShips && numShips > 0)
	{
		MoveOrder::Ptr move = MoveOrder::Ptr(new MoveOrder(from, to, numShips));
		m_self->putListMoveOrder(move);
		std::cout << "MoveOrder successful"<< std::endl;
        from->delShips(numShips);
        m_self->delShips(numShips);
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
    if(selectedPlanet->getMinesHidden() + selectedPlanet->getMinesBuild() < selectedPlanet->getMines())
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

        Planet::Ptr NewMineOnPlanet = NewOrder->getPlanet();

        NewMineOnPlanet->setMinesBuild();

        NewMineOnPlanet->resetMinesHidden();
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

        NewShipToPlanet->resetShipsOrdered();
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
    emit initMap();

}

void DataModel::setStartPlanet(std::shared_ptr<Planet> startplanet)
{
	startplanet->setOwner(m_self);
	startplanet->addShips(1);
	m_self->addPlanet(startplanet);
}

void DataModel::addMainWindow(QStackedWidget* window)
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
        m_mainWindow->window()->showFullScreen();
        emit updateInfo();
    }
    if(Id == MAIN3D)
    {
        ((MainWindow*)m_widgets[Id])->activate(true);
    }
    m_mainWindow->setCurrentWidget(m_widgets[Id]);
    
}

bool DataModel::updateAll(QJsonDocument &update) {

	if (update.isObject() && !update.isEmpty())
	{

		int id = 0;

		//int rubin = 0;
		std::string name;
		std::list<Planet::Ptr> planets;
		Player::Ptr player;

		QJsonObject all = update.object();

		if(all.empty()) return false; //QJsonDocument contains not an Object

		QJsonObject::const_iterator it;

		for (it = all.constBegin(); it != all.constEnd(); it++)
		{
			if(it.key() == "ID")
			{
				id = it.value().toInt();

				player = this->getEnemyPlayer(it.value().toInt());
			}

			if(it.key() == "Name")
			{
				name = it.value().toString().toStdString();
			}

			if(it.key() == "PlanetArray")
			{
				if(it.value().isArray())
				{
					QJsonArray array = it.value().toArray();
					QJsonArray::const_iterator it1;
					Planet::Ptr planet;
					int ships;
					int mines;

					for (it1 = array.constBegin(); it1 != array.constEnd(); it1++)
					{
						planet = getPlanetFromId(it1->toObject(QJsonObject()).value("ID").toInt());
						mines = it1->toObject(QJsonObject()).value("Mines").toInt();
						ships = it1->toObject(QJsonObject()).value("Ships").toInt();

						planet->setMines(mines);
						planet->setShips(ships);
						planet->setOwner(player);

						planets.push_back(planet);

					}//End Iterator Array

				}
				else return false; //PlanetArray is not an Array
			}
			/**
			 * when the planet is not already colonialized, the player of this file will become the owner
			 * else the player of this file becomes the invader
			 *
			 * also updates invaderShips/Ships on the planet
			 *
			 */
			if(it.key() == "InvadePlanets")
			{
				if(it.value().isArray())
				{
					QJsonArray array = it.value().toArray();
					QJsonArray::const_iterator it1;

					Planet::Ptr planet;

					int ships;

					for (it1 = array.constBegin(); it1 != array.constEnd(); it1++)
					{
						planet = getPlanetFromId(it1->toObject(QJsonObject()).value("ID").toInt());
						ships = it1->toObject(QJsonObject()).value("Ships").toInt();

						if(planet->getOwner() == nullptr || planet->getOwner()->getIdentity() == id)
						{
							planet->setOwner(player);
							planet->setShips(ships);
						}
						else
						{
							planet->setInvader(player);
							planet->setInvaderShips(ships);
						}
					}

				}
				else return false; //InvadePlanets is not an Array
			}

		}//End Iterator File

		player->setPlanetsList(planets);
	}
	return true;

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
        Planet::Ptr Planets = it->second;
        if(Planets->getInvader() != NULL)
        {
            // In case the defender ships are not present
            if(Planets->getShips() == 0)
            {
                Player::Ptr Tempplayer = Planets->getOwner();
                Tempplayer->RemovePlaneteFromList(Planets);
                Planets->setOwner(Planets->getInvader());
                Planets->getOwner()->addPlanet(Planets);
                Planets->addShips(Planets->getInvaderShips());
                

            }
            else
            {
                //everything for battle is ready
                m_battles.push_back(std::shared_ptr<Battle>(new Battle(Planets, Planets->getOwner(), 
                                Planets->getInvader(), Planets->getShips(),
                                Planets->getInvaderShips(), true)));
                std::cout << "Kampf Karte erzeugt" << std::endl;
            }
            Planets->setInvader(NULL);
        }
    }
}

QJsonDocument DataModel::createJson(Player::Ptr player)
{
    // main QJson object in the document
    QJsonObject main;

    //insert id of the player
    main.insert("ID", player->getIdentity());
    // Playername
    main.insert("Name",  QString::fromStdString(player->getPlayerName()));
    // amount of rubin
    main.insert("Rubin", player->getIdentity());

    //Json array
    QJsonArray planeets;
    
    //Planets of the player
    std::list<std::shared_ptr<Planet>> planetos = player->getPlanets();

    //Iterate over all planets and add the to the json file
    for(std::list<std::shared_ptr<Planet>>::iterator it = planetos.begin(); it != planetos.end(); ++it)
    {
        //The Planet and the qjson representations
        Planet::Ptr planet = *it;
        QJsonObject qPlanet;

        // Add necessary information to representation
        qPlanet.insert("ID", getIDFromPlanet(planet));
        qPlanet.insert("Mines", planet->getMines());
        qPlanet.insert("Ships", planet->getShips());

        //Add to the json array
        planeets.push_back(qPlanet);
    }

    //Add the array to the json file
    main.insert("PlanetArray", planeets);

    //Json array for invasion
    QJsonArray qInvasions;

    //Go over all planets and if there is a fight, add it to json file 
    for(std::map<int, Planet::Ptr>::iterator it = m_planets.begin(); it != m_planets.end(); it++)
    {
        //A planet in the list
        Planet::Ptr planet = it->second;
        // In this case an invasion is happening on this planet
        if(planet->getInvader() != NULL)
        {
            //planet representation
            QJsonObject qInvasion;

            qInvasion.insert("ID", getIDFromPlanet(planet));
            qInvasion.insert("InvaderShips", planet->getInvaderShips());

            qInvasions.push_back(qInvasion);
        }
    }

    main.insert("InvadePlanets", qInvasions);

    // Make qjsondocument out of it
    QJsonDocument theDocument(main);

    // In case the json document should be printed
    //std::cout << theDocument.toJson().toStdString() << std::endl;

    return theDocument;
}

/*
QJsonDocument createJsonOrders(Player::Ptr player)
{
    // main QJson object in the document
    QJsonObject main;

    //insert id of the player
    main.insert("ID", player->getIdentity());
    // Playername
    main.insert("Name",  QString::fromStdString(player->getPlayerName()));
    // amount of rubin
    main.insert("Rubin", player->getIdentity());

    //Json array
    QJsonArray qMineOrders;

    //Mineorders of the player
    std::list<MineOrder::Ptr> mineOrders = player->getListMineOrder();

    //Iterate over all planets and add the to the json file
    for(std::list<MineOrder::Ptr>::iterator it = mineOrders.begin(); it != mineOrders.end(); ++it)
    {
        // Get mineorder and representation
        MineOrder::Ptr mineOrder = *it;
        QJsonObject qMineOrder;

        //Add Id
        //qMineOrder.insert("PlanetID", getIDFromPlanet(mineOrder->getPlanet()));

        qMineOrders.push_back(qMineOrder);
       
        //The Planet and the qjson representations
        Planet::Ptr planet = *it;
        QJsonObject qPlanet;

        // Add necessary information to representation
        qPlanet.insert("ID", getIDFromPlanet(planet));
        qPlanet.insert("Mines", planet->getMines());
        qPlanet.insert("Ships", planet->getShips());

        //Add to the json array
        planeets.push_back(qPlanet);
        
    }

    return QJsonDocument();
}
*/

void DataModel::performMovements(Player::Ptr player)
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
        if(destination->getOwner()== 0)
        {
            Player::Ptr Empty = Player::Ptr(new Player());
            destination->setOwner(Empty);
        }
        std::cout << destination->getOwner() << std::endl;
        std::cout << destination->getShips() << std::endl;
      
        if(m_self == destination->getOwner()){
            //the destination planet is of the same owner
            destination->setOwner(player);
            destination->addShips(ships);
       
        }else{
            //the destination planet is of another player but he might defend
            destination->setInvader(m_self);
            destination->addInvaderShips(ships);
        }
    }
}

void DataModel::BattleReport()
{
    std::list<std::shared_ptr<Battle>> BattleResult = m_battles;
    for (std::list<std::shared_ptr<Battle>>::iterator it = BattleResult.begin(); it != BattleResult.end(); ++it)
    {
        std::shared_ptr<Battle> BattleDetailResult = *it;
        if(BattleDetailResult->FightResultInvader == false)
        {
            std::cout << "Kampf verloren" << std::endl;
            BattleDetailResult->m_player2->delShips(BattleDetailResult->m_numberShips2);
        }
        if(BattleDetailResult->FightResultInvader == true)
        {
            std::cout << "Kampf gewonnen" << std::endl;
            BattleDetailResult->m_location->setOwner(BattleDetailResult->m_player2);
            BattleDetailResult->m_location->delShips(BattleDetailResult->m_numberShips1);
            BattleDetailResult->m_location->addShips(BattleDetailResult->m_numberShips2);
            BattleDetailResult->m_player2->addPlanet(BattleDetailResult->m_location);
            BattleDetailResult->m_player1->RemovePlaneteFromList(BattleDetailResult->m_location);


        }
    }
    m_battles.clear();
}

int DataModel::getIDFromPlanet(Planet::Ptr planet)
{
    // go over all planets in planets
    for(int i = 0; i < ((int) m_planets.size()); i++)
    {
        // Get planet with index i
        Planet::Ptr mapPlanet = m_planets.find(i)->second;
        // If they're the same planets correct planet has been found
        if(mapPlanet->getName() == planet->getName())
        {
            return i;
        }
    }
    
    // If we get to this point, the planet was not found in the map of all planets
    std::cerr << "Achtung, die ID des Planeten " << planet->getName() << "wurde nicht gefunden"; 
    std::cerr << "Es wurde ID 0 ausgegeben" << std::endl; 
    return 0;
}

int DataModel::getIDFromPlanetName(std::string name){
    return m_planetNameToId[name];
}

void DataModel::WinCondition()
{
    std::map<int, Planet::Ptr>::iterator it;
    int NumberOfPlanets = m_planets.size();
    int CountOfPlanets = 0;
    std::cout << "Anzahl der Planeten" << std::endl;
    std::cout << NumberOfPlanets << std::endl;
    for(it = m_planets.begin(); it != m_planets.end(); it++)  
    {
        Planet::Ptr Planets = it->second;
        if(Planets->getOwner() == m_self)
        {
            CountOfPlanets++;
        }

    }  
    std::cout << CountOfPlanets << std::endl;
    if(NumberOfPlanets == CountOfPlanets)
    {
        std::cout << "Gewonnen" <<std::endl;
        switchWindow(DataModel::END);


    }

}

void DataModel::BattlePhase()
{
    std::list<std::shared_ptr<Battle>> BattlePhase = m_battles;
    for (std::list<std::shared_ptr<Battle>>::iterator it = BattlePhase.begin(); it != BattlePhase.end(); ++it)  
    {
        std::shared_ptr<Battle> BattleDetail = *it;
        std::cout << "Kampfphase" << std::endl;
        std::cout << BattleDetail->m_location->getName() << std::endl;
        std::cout << BattleDetail->m_player1->getPlayerName() << std::endl;
        std::cout << BattleDetail->m_numberShips1 << std::endl;
        std::cout << "" << std::endl;
        std::cout << BattleDetail->m_player2->getPlayerName() << std::endl;
        std::cout << BattleDetail->m_numberShips2 << std::endl;

        


    }

}

void DataModel::addPlayer(Player::Ptr player)
{
    m_players.insert(std::pair<int, Player::Ptr>(player->getIdentity(),player));
}

Player::Ptr DataModel::getPlayerByID(int i)
{
    return m_players.find(i)->second;
}

DataModel::~DataModel()
{

}

}
