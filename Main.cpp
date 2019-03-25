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

    //strategy::StartingDialog startWindow(model);

    mainWindow2D.show();

    return a.exec();
}

