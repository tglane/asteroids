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
#include "view2D/MainWindow2D.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "Usage: asteroids <level-file>" << endl;
        return 1;
    }

    QApplication a(argc, argv);

    /*asteroids::MainWindow mainWindow(argv[1]);
    mainWindow.show();*/

    strategy::MainWindow2D mainWindow2D;
    mainWindow2D.show();

    return a.exec();
}
