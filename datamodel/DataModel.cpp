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
    // m_enemy = Player::Ptr(new Player());

    // when networking issues are solved the map is loaded later
    getUniverse(filename);

    Planet::Ptr Test = getPlanetFromId(5);
 	Planet::Ptr Test2 = getPlanetFromId(6);
	Planet::Ptr Test3 = getPlanetFromId(7);
	m_enemy = Player::Ptr(new Player(1,3000,0));
	
    Test->setOwner(m_enemy);
    Test2->setOwner(m_enemy);
    Test3->setOwner(m_enemy);
    Test3->addShips(3);
	m_enemy->addPlanet(Test);
	m_enemy->addPlanet(Test2);
	m_enemy->addPlanet(Test3);
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

    performMovements(getSelfPlayer());
    findBattles();
    BattleReport();

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

	if(from->getShips() >= numShips)
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
//TODO ordentliche Fehlerbehandlung + Doku + manche (unnötige) Felder in Player koennen mit Infos aus File nicht aktualisiert werden
bool DataModel::updateAll(QJsonDocument &update) {

	if (update.isObject() && !update.isEmpty())
	{

		int id;
		std::string name;
		std::list<Planet::Ptr> planets;
		Player::Ptr player;

		QJsonObject all = update.object();
		//all leeres Object, falls QJsonDokument Array und kein Object ist
		if(all.empty()) return false;

		QJsonObject::const_iterator it;
		for (it = all.constBegin(); it != all.constEnd(); it++)
		{
			if(it.key() == "ID")
			{
				id = it.value().toInt();
				//TODO Later getPlayerByID?
				player = m_enemy;

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

						planets.push_back(planet);

					}//End Iterator Array
				}
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
            // In case the defender ships are not present anymore
            if(Planets->getShips() == 0)
            {
                Planets->setOwner(Planets->getInvader());
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

    //Iterate over all and add the to the json file
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

    // Make qjsondocument out of it
    QJsonDocument theDocument(main);

    // In case the json document should be printed
    //std::cout << theDocument.toJson().toStdString() << std::endl;

    return theDocument;
}


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
       
        }else if(destination->getShips()==0){
            //the destination planet is of another player but he cannot defend, planet acquired
            destination->setOwner(m_self);
            destination->addShips(ships);

            m_self->addPlanet(destination);
            std::cout <<"Speicherzugriffsfehler test before" << std::endl;
            destination->setInvader(NULL);
            std::cout <<"Speicherzugriffsfehler test" << std::endl;
            destination->setInvaderShips(0);
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


        }
    }
    m_battles.clear();
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
