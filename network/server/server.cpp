#include "server.h"
#include <QNetworkDatagram>
#include <iostream>
#include <thread>
#include <chrono>

Server::Server()
{
    socket = std::unique_ptr<QUdpSocket>(new QUdpSocket(nullptr));
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket.get(), &QUdpSocket::readyRead, this, &Server::handle_udp);
}

asteroids::Vector3f Server::bytes_to_vector(char *bytes)
{
    asteroids::Vector3f vec;
    for (int i = 0; i < 3; i++) {
        vec[i] = *(float *)(bytes + i * 4);
    }
    return vec;
}

void Server::handle_position_packet(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    uint32_t obj_id = *(data.data() + 1);
    std::cout << "received position data:" << std::endl;
    obj_id &= 0xff000000;
    if (objects.count(obj_id) != 1) {
        std::cout << "invalid obj_id" << std::endl;
        //return;
    }
    //light_object &obj = objects.at(obj_id);
    asteroids::Vector3f position = bytes_to_vector(data.data() + 9);
    asteroids::Vector3f velocity = bytes_to_vector(data.data() + 9 + 3 * 4);
    asteroids::Vector3f orientation = bytes_to_vector(data.data() + 9 + 6 * 4);
    // obj.set_position(position);
    // obj.set_velocity(velocity);
    // obj.set_orientation(orientation);
    std::cout << "id: " << obj_id 
              << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2]
              << " v: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2]
              << " o: " << orientation[0] << ", " <<  orientation[1] << ", " <<  orientation[2] << std::endl;
}

void Server::handle_bullet_packet(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    handle_position_packet(datagram);
    uint32_t obj_id = *(data.data() + 1);
    
}


void Server::handle_udp()
{
    while(socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray data = datagram.data();

        if (data[0] == 'A') {
            uint32_t seq_nr = *(data.data() + 1);
            std::cout << "received ACK " << seq_nr << std::endl;
            return;
        }

        // TODO send acks
        switch (data[0]) {
        case 'P':
            std::cout << "received POS" << std::endl;
            handle_position_packet(datagram);
            break;
        case 'B':
            handle_bullet_packet(datagram);
            break;
        default:
            std::cout << "received unknown packet" << std::endl;
        }
    }
}


void Server::run()
{
}


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Server server;
    return app.exec();
}
