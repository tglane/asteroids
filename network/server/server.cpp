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
        while(socket->hasPendingDatagrams())
        {
            QNetworkDatagram datagram = socket->receiveDatagram();
            QByteArray data = datagram.data();
            std::cout << "received: " << data << std::endl;

            // überprüfe header
            if (data.startsWith("POS")) {
            } else if (data.startsWith("")) {
            }
            // rufe behandlungroutine auf

            // rufe movement aus
        }

        std::cout << "safd" << std::endl;
        // schicke Zustand an clienten
        std::this_thread::sleep_for(sleep_time);
    }
}

int main()
{
    Server server = Server();
    server.run();
}
