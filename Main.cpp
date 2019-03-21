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
    /*if (argc < 3)
    {
        cerr << "Usage: asteroids <level-file> <map-file>" << endl;
        return 1;
    }*/

    DataModel::Ptr model = DataModel::Ptr(new DataModel("../models/Level-1.txt"));

    QApplication a(argc, argv);

    strategy::MainWindow2D mainWindow2D(model);

    //asteroids::MainWindow mainWindow("../models/level.xml");
    //mainWindow.show();

    strategy::StartingDialog startWindow(model);
    startWindow.show();


    return a.exec();
}

