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

    strategy::MainWindow2D mainWindow2D(&model);
    mainWindow2D.show();
    mainWindow2D.showFullScreen();

    return a.exec();
}
