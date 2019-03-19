#include "server.h"
#include <QNetworkDatagram>
#include <iostream>
#include <thread>
#include <memory>
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
    std::cout << "received position data:" << std::endl;
    QByteArray data = datagram.data();
    if (data.length() < 9 + 9 * 4) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    uint32_t obj_id = *(data.data() + 1);
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

void Server::send_ack(QNetworkDatagram &datagram)
{

    if (datagram.data().length() < 5) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    char* data = datagram.data().data();
    uint32_t seq_nr = *(data + 1);
    std::cout << "sending ACK seq_nr: " << seq_nr << std::endl;
    QByteArray reply_data;
    reply_data.append('A');
    reply_data.append((char *)(&seq_nr), 4);

    //sprintf(target, "%04d", seq_number++ );
    //double r = rand() / (RAND_MAX + 1.);
    //sprintf(target, "%04d", seq_number++ );

    socket->writeDatagram(datagram.makeReply(reply_data));
}

void Server::send_collision(Client &client, uint32_t obj_id1, uint32_t obj_id2)
{
    uint32_t seq_nr = client.next_seq_nr();
    QByteArray &data = client.ack_pending[seq_nr];
    data.append('C');
    data.append((char *)(&seq_nr), 4);
    data.append((char *)(&obj_id1), 4);
    data.append((char *)(&obj_id2), 4);
    socket->writeDatagram(data, client.address, client.port);
}

void Server::handle_ack(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    uint32_t client_id = *(data.data() + 5) >> 24;
    Client &client = clients[client_id];
    uint32_t seq_nr = *(data.data() + 1);
    if (client.ack_pending.erase(seq_nr) != 1) {
        std::cout << "invalid seq_nr" << std::endl;
        return;
    }
}

bool Server::check_client_id(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    if (data.length() < 9) {
        std::cout << "packet to short" << std::endl;
        return false;
    }
    uint32_t client_id = *(data.data() + 5) >> 24;
    if (clients.count(client_id) != 1) {
        std::cout << "invalid client_id" << std::endl;
        return false;
    }
    Client &client = clients[client_id];
    if (client.address != datagram.senderAddress() || client.port != datagram.senderPort()) {
        std::cout << "wrong client_id" << std::endl;
        return false;
    }
    return true;
}


void Server::handle_udp()
{
    while(socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray data = datagram.data();

        if (!check_client_id(datagram)) {
            continue;
        }

        if (data[0] == 'A') {
            handle_ack(datagram);
            continue;
        }

        send_ack(datagram);

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
