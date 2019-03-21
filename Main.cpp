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



int main(int argc, char** argv)
{
    /*if (argc < 3)
    {
        cerr << "Usage: asteroids <level-file> <map-file>" << endl;
        return 1;
    }*/

    DataModel model("../models/Level-1.txt");

    QApplication a(argc, argv);

    /*asteroids::MainWindow mainWindow(argv[1]);
    mainWindow.show();*/

    /*Test für buyShip und buyMine*/
    Player::Ptr Testplayer = Player::Ptr(new Player(1,2000,0));
    std::cout << Testplayer->getIdentity() << std::endl;
    std::cout << Testplayer->getPlayerName() << std::endl;
    Testplayer->setPlayerName("Huber van Windrad");
    std::cout << Testplayer->getRubin() << std::endl;
    std::cout << Testplayer->getPlayerName() << std::endl;

    Planet::Ptr Test = model.getPlanetFromId(5);
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMines() << std::endl;
    bool buytest;

    bool buytest2;
    buytest = model.buyShip(Test, Testplayer);
    buytest2 = model.buyMine(Test, Testplayer);

    buytest = model.buyShip(Test, Testplayer);
    model.TransaktionShip(Testplayer);
    model.TransaktionMine(Testplayer);
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMines() << std::endl;
    model.clearOrderList(Testplayer);
    model.TransaktionShip(Testplayer);
    model.TransaktionMine(Testplayer);
    std::cout << Test->getName() << std::endl;
    std::cout << Test->getShips() << std::endl;
    std::cout << Test->getMines() << std::endl;

    





    strategy::MainWindow2D mainWindow2D(&model);
    mainWindow2D.show();

    return a.exec();
}
