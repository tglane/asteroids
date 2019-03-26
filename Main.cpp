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

#include "network/client/tcpclient.hpp"
#include "network/client/udpclient.hpp"



int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    DataModel::Ptr model = std::make_shared<DataModel>();
    model->getUniverse("../models/Level-01.map");

    //TODO add input for player name and server io
//    tcpclient::Ptr tcp_client(std::make_shared<tcpclient>(model));
//    QAbstractSocket::connect(model.get(), SIGNAL(endround_signal()), tcp_client.get(), SLOT(send_ready()));

    strategy::GameWindow gamewindow(model);

    gamewindow.show();

    return a.exec();
}

