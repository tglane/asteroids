#include "DataModel_Server.hpp"
#include <iostream>
#include <fstream>
#include <utility>


namespace asteroids{

DataModel_Server::DataModel_Server() : m_players(), m_planets(), m_edges()
{}

DataModel_Server::DataModel_Server(std::string filename) : m_players(), m_planets(), m_edges()
{
    getUniverse(filename);
}

DataModel_Server::DataModel_Server(std::string filename, int id, std::string player_name ) : m_players(), m_planets(), m_edges()
{
    m_self = Player::Ptr(new Player(id, 3000, 0, player_name));
    addPlayer(m_self);
    getUniverse(filename);
}

void DataModel_Server::constructPlayer(int id, std::string player_name, bool is_self)
{
    // kein Name könnte Problem sein
    Player::Ptr p(new Player(id, 3000, 0, player_name));
    if (is_self) {
        m_playerid = id;
        m_self = p;
    } else {
        m_enemy = p;
    }
    addPlayer(p);
}

void DataModel_Server::getUniverse(std::string filename)
{

    std::ifstream f;
    f.open(filename);

    if(f.is_open())
    {
        int numvertex, posx, posy, mines;
        std::string name;

        f >> numvertex >> posx >> posy;

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

std::map<int, Planet::Ptr> DataModel_Server::getPlanets()
{
    return m_planets;
}

std::list<std::pair<int,int>> DataModel_Server::getEdges()
{
    return m_edges;
}

bool DataModel_Server::endOfRound()
{
    std::cout << "End of Round!" << std::endl;

    performMovements(getSelfPlayer());
    //findBattles();
    //BattlePhase();
    m_self->PrintPlanetsList();
    m_enemy->PrintPlanetsList();
    //BattleReport();
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
bool DataModel_Server::buyShip(Planet::Ptr selectedPlanet, Player::Ptr m_self)
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

bool DataModel_Server::moveShips(Planet::Ptr from, Planet::Ptr to, int numShips) {

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
bool DataModel_Server::buyMine(Planet::Ptr selectedPlanet, Player::Ptr m_self)
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

void DataModel_Server::TransaktionMine()
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

void DataModel_Server::TransaktionShip()
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

void DataModel_Server::clearOrderList()
{
    m_self->ClearOrderListInPlayer();
}

Planet::Ptr DataModel_Server::getPlanetFromId(int ID)
{
    return m_planets.at(ID);
}

Planet::Ptr DataModel_Server::getPlanetFromName(std::string name)
{
    return m_nameToPlanets[name];
}

void DataModel_Server::calculateFinance(Player::Ptr Player)
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
  
void DataModel_Server::startGame()
{
    emit initMap();

}

void DataModel_Server::setStartPlanet(std::shared_ptr<Planet> startplanet)
{
	startplanet->setOwner(m_self);
	startplanet->addShips(1);
	m_self->addPlanet(startplanet);
}

void DataModel_Server::clearInvaders() {
    for (auto i: m_planets) {
        i.second->setInvaderShips(0);
        i.second->setInvader(nullptr);
    }
}


bool DataModel_Server::updateAll(QJsonObject &update) {

	//if (update.isObject() && !update.isEmpty())
	//{
    int id = 0;

    //int rubin = 0;
    std::string name;
    std::list<Planet::Ptr> planets;
    Player::Ptr player;

    QJsonObject all = update;

    if(all.empty()) return false; //QJsonDocument contains not an Object


    id = update["ID"].toInt();
    player = this->getPlayerByID(id);
    name = update["Name"].toString().toStdString();
    if(update["PlanetArray"].isArray())
    {
        QJsonArray array = update["PlanetArray"].toArray();
        QJsonArray::const_iterator it1;
        Planet::Ptr planet;
        int ships;
        int mines;


        for (it1 = array.constBegin(); it1 != array.constEnd(); it1++)
        {
            std::cout << it1->toObject(QJsonObject()).value("ID").toInt() << std::endl;
            planet = getPlanetFromId(it1->toObject(QJsonObject()).value("ID").toInt());
            mines = it1->toObject(QJsonObject()).value("Mines").toInt();
            ships = it1->toObject(QJsonObject()).value("Ships").toInt();

            planet->setMines(mines);
            planet->setShips(ships);
            planet->setOwner(player);

            planets.push_back(planet);

        }//End Iterator Array

    }
    /**
     * when the planet is not already colonialized, the player of this file will become the owner
     * else the player of this file becomes the invader
     *
     * also updates invaderShips/Ships on the planet
     *
     */
    if(update["InvadePlanets"].isArray())
    {

        QJsonArray::const_iterator it1;

        QJsonArray array = update["InvadePlanets"].toArray();

        Planet::Ptr planet;

        int ships;

        for (it1 = array.constBegin(); it1 != array.constEnd(); it1++)
        {
            planet = getPlanetFromId(it1->toObject(QJsonObject()).value("ID").toInt());
            ships = it1->toObject(QJsonObject()).value("InvaderShips").toInt();

            
            if (planet->getOwner() == nullptr) {
                planet->setOwner(player);
                planet->setShips(ships);
                planets.push_back(planet);
            }
            else if(planet->getOwner()->getIdentity() == id)
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

		//}//End Iterator File

    player->setPlanetsList(planets);

    emit updateInfo();
	return true;
}

Player::Ptr DataModel_Server::getSelfPlayer()
{
    return m_self;
}

Player::Ptr DataModel_Server::getEnemyPlayer()
{
    return m_enemy;
}

std::vector<Battle::Ptr> DataModel_Server::findBattles()
{
    m_battles.clear();
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
    return m_battles;
}

int DataModel_Server::getIDFromPlanet(Planet::Ptr planet)
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

void DataModel_Server::printPlanets()
{
     for (auto i: m_planets) {
        std::cout << "planet: "
                 << i.first
                 << " " << i.second->getName();
        if (i.second->getOwner() != nullptr) {
            std::cout << " owner: " << i.second->getOwner()->getIdentity();
        }
        if (i.second->getInvader() != nullptr) {
            std::cout << " invader: " << i.second->getInvader()->getIdentity();
        }
        std::cout << std::endl;
    }
}

QJsonObject DataModel_Server::createJson(Player::Ptr player)
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
    //QJsonDocument theDocument(main);

    // In case the json document should be printed
    //std::cout << theDocument.toJson().toStdString() << std::endl;

    return main;
}

QJsonObject DataModel_Server::createBattleJson(Battle::Ptr battle)
{
    // main QJson object in the document
    QJsonObject main;
    
    main.insert("locationID", getIDFromPlanet(battle->m_location));
    main.insert("playerID1", battle->m_player1->getIdentity());
    main.insert("playerID2", battle->m_player2->getIdentity());
    main.insert("numberShips1", battle->m_numberShips1);
    main.insert("numberShips2", battle->m_numberShips2);
    main.insert("numberShipsLost1", battle->m_numberShipsLost1);
    main.insert("numberShipsLost2", battle->m_numberShipsLost2);
    main.insert("invaderWon", battle->FightResultInvader);

    return main;
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

void DataModel_Server::performMovements(Player::Ptr player)
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

void DataModel_Server::BattleReport()
{
    std::vector<std::shared_ptr<Battle>> BattleResult = m_battles;
    for (std::vector<std::shared_ptr<Battle>>::iterator it = BattleResult.begin(); it != BattleResult.end(); ++it)
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

int DataModel_Server::getIDFromPlanetName(std::string name){
    return m_planetNameToId[name];
}

void DataModel_Server::WinCondition()
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


    }
}

void DataModel_Server::BattlePhase()
{
    std::vector<std::shared_ptr<Battle>> BattlePhase = m_battles;
    for (std::vector<std::shared_ptr<Battle>>::iterator it = BattlePhase.begin(); it != BattlePhase.end(); ++it)  
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

void DataModel_Server::addPlayer(Player::Ptr player)
{
    m_players.insert(std::pair<int, Player::Ptr>(player->getIdentity(),player));
}

Player::Ptr DataModel_Server::getPlayerByID(int i)
{
    // Playerliste leer?
    if(m_players.empty())
    {
        std::cerr << "Playerliste ist leer, es kann noch kein spieler gefunden werden" << std::endl;
        return Player::Ptr(new Player(-1));
    }
    std::map<int, Player::Ptr>::iterator it = m_players.find(i);
    // Player in Liste?
    if(m_players.end() == it)
    {
        std::cerr << "Spieler wurde anhand von ID nicht gefunden" << std::endl;
        return Player::Ptr(new Player(-1));
    }
    return it->second;
}

DataModel_Server::~DataModel_Server()
{

}

}
