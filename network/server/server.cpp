#include "server.h"
#include <QNetworkDatagram>
#include <iostream>
#include <thread>
#include <chrono>

Server::Server()
{
    socket = std::unique_ptr<QUdpSocket>(new QUdpSocket(nullptr));
    socket->bind(QHostAddress::LocalHost, 1234);
}

void Server::run()
{
    std::chrono::milliseconds sleep_time(1000 / 60);

    while(1)
    {
        // rufe movement aus
        for(auto obj: this->objects) {
            //obj->move()
        }


        while(socket->hasPendingDatagrams())
        {
            QNetworkDatagram datagram = socket->receiveDatagram();
            QByteArray data = datagram.data();
            std::cout << "received: " << data[0] << std::endl;

            // überprüfe header
            if (data.startsWith("POS")) {
                // check player position
                // update player position
                // send player position to other clients
            } else if (data.startsWith("")) {

            }
            // rufe behandlungroutine auf
        }
        //std::cout << "safd" << std::endl;
// schicke Zustand an clienten

        std::this_thread::sleep_for(sleep_time);
    }
}

int main()
{
    Server server = Server();
    server.run();
}
