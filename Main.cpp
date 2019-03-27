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
#include "view2D/GameWindow.hpp"



int main(int argc, char** argv)
{

    DataModel::Ptr model = DataModel::Ptr(new DataModel("../models/Level-2.map"));

    QApplication a(argc, argv);

    strategy::GameWindow gamewindow(model);

    gamewindow.show();

    return a.exec();
}

