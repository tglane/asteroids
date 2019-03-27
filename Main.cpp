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

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "Usage: asteroids <level-file>" << endl;
        return 1;
    }

    QApplication a(argc, argv);

    asteroids::MainWindow mainWindow(argv[1]);
    mainWindow.showFullScreen();
    mainWindow.show();

    return a.exec();
}
