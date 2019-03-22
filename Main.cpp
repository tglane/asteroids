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
    Player::Ptr Testplayer = Player::Ptr(new Player(1,2000,0));
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMines() << std::endl;
    bool buytest;

    bool buytest2;
    buytest = model->buyShip(Test, Testplayer);
    buytest2 = model->buyMine(Test, Testplayer);

    buytest = model->buyShip(Test, Testplayer);
    model->TransaktionShip(Testplayer);
    model->TransaktionMine(Testplayer);
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMines() << std::endl;
    model->clearOrderList(Testplayer);
    model->TransaktionShip(Testplayer);
    model->TransaktionMine(Testplayer);
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMines() << std::endl;

    





    strategy::StartingDialog startWindow(model);
    startWindow.show();

    //mainWindow2D.showFullScreen();

    return a.exec();
}

