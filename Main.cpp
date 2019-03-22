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

    DataModel::Ptr model = DataModel::Ptr(new DataModel("../models/Level-1.txt"));

    QApplication a(argc, argv);

    strategy::MainWindow2D mainWindow2D(model);

    //asteroids::MainWindow mainWindow("../models/level.xml");
    //mainWindow.show();

    Planet::Ptr Test = model->getPlanetFromId(5);
    Planet::Ptr Test2 = model->getPlanetFromId(6);
    Planet::Ptr Test3 = model->getPlanetFromId(7);
    Player::Ptr Testplayer = Player::Ptr(new Player(1,3000,0));
    Testplayer->addPlanet(Test);
    Testplayer->addPlanet(Test2);
    Testplayer->addPlanet(Test3);
    std::cout << "Player mit Name" << std::endl;
    std::cout << Testplayer->getPlayerName() << std::endl;
    std::cout << Testplayer->getRubin() << std::endl;
    std::cout << "Planet mit Name" << std::endl;
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMinesBuild() << std::endl;
    std::cout << "Planet mit Name 2" << std::endl;
    std::cout << Test2->getName() << std::endl;
    std::cout << Test2->getShips() << std::endl;
    std::cout << Test2->getMinesBuild() << std::endl;
    bool buytest;

    bool buytest2;
    buytest = model->buyShip(Test, Testplayer);
    buytest2 = model->buyMine(Test, Testplayer);
    buytest2 = model->buyMine(Test, Testplayer);

    buytest = model->buyShip(Test, Testplayer);
    model->calculateFinance(Testplayer);

    buytest2 = model->buyMine(Test2, Testplayer);
    std::cout << "" << std::endl;
    std::cout << "Player mit Name" << std::endl;
    std::cout << Testplayer->getPlayerName() << std::endl;
    std::cout << Testplayer->getRubin() << std::endl;   
    std::cout << "Planet mit Name" << std::endl;
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMinesBuild() << std::endl;
    std::cout << "Planet mit Name 2" << std::endl;
    std::cout << Test2->getName() << std::endl;
    std::cout << Test2->getShips() << std::endl;
    std::cout << Test2->getMinesBuild() << std::endl;

    model->calculateFinance(Testplayer);
    std::cout << "" << std::endl;
    std::cout << "Player mit Name" << std::endl;
    std::cout << Testplayer->getPlayerName() << std::endl;
    std::cout << Testplayer->getRubin() << std::endl;   
    std::cout << "Planet mit Name" << std::endl;
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMinesBuild() << std::endl;
    std::cout << "Planet mit Name 2" << std::endl;
    std::cout << Test2->getName() << std::endl;
    std::cout << Test2->getShips() << std::endl;
    std::cout << Test2->getMinesBuild() << std::endl;

    model->calculateFinance(Testplayer);
    std::cout << "" << std::endl;
    std::cout << "Player mit Name" << std::endl;
    std::cout << Testplayer->getPlayerName() << std::endl;
    std::cout << Testplayer->getRubin() << std::endl;   
    std::cout << "Planet mit Name" << std::endl;
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMinesBuild() << std::endl;
    std::cout << "Planet mit Name 2" << std::endl;
    std::cout << Test2->getName() << std::endl;
    std::cout << Test2->getShips() << std::endl;
    std::cout << Test2->getMinesBuild() << std::endl;


    





    strategy::StartingDialog startWindow(model);
    startWindow.show();

    //mainWindow2D.showFullScreen();

    return a.exec();
}

