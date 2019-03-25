/*
 *  Main.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "view/MainWindow.hpp"
#include "datamodel/DataModel.hpp"
#include "datamodel/Player.hpp"
#include "view2D/MainWindow2D.hpp"
#include "view2D/StartingDialog.hpp"



int main(int argc, char** argv)
{

    DataModel::Ptr model = DataModel::Ptr(new DataModel("../models/Level-1.map"));

    QApplication a(argc, argv);


    strategy::MainWindow2D mainWindow2D(model);

    //asteroids::MainWindow mainWindow("../models/level.xml");
    //mainWindow.show();

    Planet::Ptr Test = model->getPlanetFromId(5);
 	Planet::Ptr Test2 = model->getPlanetFromId(6);
	Planet::Ptr Test3 = model->getPlanetFromId(7);
	Player::Ptr Testplayer = Player::Ptr(new Player(1,3000,0));
	
    Test->setOwner(Testplayer);
    Test2->setOwner(Testplayer);
    Test3->setOwner(Testplayer);
    Test3->addShips(3);
	Testplayer->addPlanet(Test);
	Testplayer->addPlanet(Test2);
	Testplayer->addPlanet(Test3);
	
	    std::cout << "Player mit Name" << std::endl;
	
	    std::cout << Testplayer->getPlayerName() << std::endl;
	
	    std::cout << Testplayer->getRubin() << std::endl;
	
	    std::cout << "Planet mit Name" << std::endl;
	
	    std::cout << Test->getName() << std::endl;
	
	    std::cout << Test->getShips() << std::endl;
    // Test
    /*
    model->getSelfPlayer()->addPlanet(model->getPlanets().find(1)->second);
    model->getSelfPlayer()->addPlanet(model->getPlanets().find(2)->second);
    model->getSelfPlayer()->addPlanet(model->getPlanets().find(3)->second);
    model->createJson(model->getSelfPlayer());
    */
    //end test

    strategy::StartingDialog startWindow(model);
    startWindow.show();

    return a.exec();
}

