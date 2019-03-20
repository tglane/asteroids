#include "UdpServer.hpp"

#include <QNetworkDatagram>
#include <iostream>
#include <thread>
#include <memory>
#include <chrono>
#include <light_bullet.hpp>

UdpServer::UdpServer()
{
    socket = std::unique_ptr<QUdpSocket>(new QUdpSocket(nullptr));
    socket->bind(QHostAddress(QString("0.0.0.0")), 1235);
    connect(socket.get(), &QUdpSocket::readyRead, this, &UdpServer::handle_udp);

    timer = std::unique_ptr<QTimer>(new QTimer(nullptr));
    connect(timer.get(), &QTimer::timeout, this, &UdpServer::tick);
    timer->start(1000/10);

    clients[42] = UdpClient(42);
    clients[42].address = QHostAddress::LocalHost;
    clients[42].port = 1234;

    clients[43] = UdpClient(43);
    clients[43].address = QHostAddress(QString("192.168.43.103"));
    clients[43].port = 1234;
}



asteroids::Vector3f UdpServer::bytes_to_vector(char *bytes)
{
    asteroids::Vector3f vec;
    for (int i = 0; i < 3; i++) {
        vec[i] = *((float *)(bytes + i * 4));
    }
    return vec;
}


void UdpServer::set_position_from_packet(QNetworkDatagram &datagram, light_object &obj)
{
    std::cout << "received position data:" << std::endl;
    QByteArray data = datagram.data();
    if (data.length() < 9 + 15 * 4) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    asteroids::Vector3f position = bytes_to_vector(data.data() + 9);
    asteroids::Vector3f velocity = bytes_to_vector(data.data() + 9 + 3 * 4);
    asteroids::Vector3f x_axis = bytes_to_vector(data.data() + 9 + 6 * 4);
    asteroids::Vector3f y_axis = bytes_to_vector(data.data() + 9 + 9 * 4);
    asteroids::Vector3f z_axis = bytes_to_vector(data.data() + 9 + 12 * 4);
    obj.set_position(position);
    obj.set_velocity(velocity);
    obj.set_x_axis(x_axis);
    obj.set_y_axis(y_axis);
    obj.set_z_axis(z_axis);
    std::cout << "id: " << obj.get_id()
              << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
              << " v: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl
              << " x: " << x_axis[0] << ", " <<  x_axis[1] << ", " <<  x_axis[2] << std::endl
              << " y: " << y_axis[0] << ", " <<  y_axis[1] << ", " <<  y_axis[2] << std::endl
              << " z: " << z_axis[0] << ", " <<  z_axis[1] << ", " <<  z_axis[2] << std::endl;
}

void UdpServer::handle_position_packet(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    if (data.length() < 9 + 9 * 4) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    uint32_t client_id = *((uint32_t*)(data.data() + 5));
    client_id = client_id >> 24;
    if (clients.count(client_id) != 1) {
        std::cout << "invalid client_id: " << client_id << std::endl;
        return;
    }

    set_position_from_packet(datagram, clients[client_id].ship);
}

void UdpServer::handle_bullet_packet(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    if (data.length() < 9 + 9 * 4) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    uint32_t obj_id = *((uint32_t*)(data.data() + 5));
    objects[obj_id] = std::unique_ptr<light_object>(new light_bullet(obj_id));
    std::cout << "created bullet: " << obj_id << std::endl;
    set_position_from_packet(datagram, *objects[obj_id]);

    for (auto& i: clients) {
        UdpClient& dest = i.second;
        send_position_or_bullet('B', dest, *objects[obj_id]);
    }
}

void UdpServer::send_ack(QNetworkDatagram &datagram)
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

    socket->writeDatagram(datagram.makeReply(reply_data));
}

void UdpServer::send_collision(UdpClient &client, uint32_t obj_id1, uint32_t obj_id2)
{
    uint32_t seq_nr = client.next_seq_nr();
    QByteArray &data = client.ack_pending[seq_nr];
    uint32_t client_id = 0;
    data.append('C');
    data.append((char *)(&seq_nr), 4);
    data.append((char *)(&client_id), 4);
    data.append((char *)(&obj_id1), 4);
    data.append((char *)(&obj_id2), 4);
    socket->writeDatagram(data, client.address, client.port);
}

void UdpServer::send_position_or_bullet(char type, UdpClient &client, light_object &obj)
{
    uint32_t seq_nr = client.next_seq_nr();
    std::cout << "sending  " << type << " to " << client.id << " seq_nr " << seq_nr << std::endl;
    QByteArray data;
    uint32_t obj_id = obj.get_id();

    data.append(type);
    data.append((char *)(&seq_nr), 4);
    data.append((char *)(&obj_id), 4);

    asteroids::Vector3f position = obj.get_position();
    asteroids::Vector3f velocity = obj.get_velocity();
    asteroids::Vector3f x_axis = obj.get_x_axis();
    asteroids::Vector3f y_axis = obj.get_y_axis();
    asteroids::Vector3f z_axis = obj.get_z_axis();


    data.append((char *)(&position[0]), 4);
    data.append((char *)(&position[1]), 4);
    data.append((char *)(&position[2]), 4);

    data.append((char *)(&velocity[0]), 4);
    data.append((char *)(&velocity[1]), 4);
    data.append((char *)(&velocity[2]), 4);

    data.append((char *)(&x_axis[0]), 4);
    data.append((char *)(&x_axis[1]), 4);
    data.append((char *)(&x_axis[2]), 4);

    data.append((char *)(&y_axis[0]), 4);
    data.append((char *)(&y_axis[1]), 4);
    data.append((char *)(&y_axis[2]), 4);

    data.append((char *)(&z_axis[0]), 4);
    data.append((char *)(&z_axis[1]), 4);
    data.append((char *)(&z_axis[2]), 4);

    socket->writeDatagram(data, client.address, client.port);
    if (type == 'B') {
        client.ack_pending[seq_nr] = data;
    }
}

void UdpServer::handle_ack(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    uint32_t client_id = *((uint32_t*)(data.data() + 5)) >> 24;
    UdpClient &client = clients[client_id];
    uint32_t seq_nr = *((uint32_t*)(data.data() + 1));
    if (client.ack_pending.erase(seq_nr) != 1) {
        std::cout << "invalid seq_nr" << std::endl;
    } else {
        std::cout << "seq_nr: " << seq_nr << std::endl;
    }
}

bool UdpServer::check_client_id(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    if (data.length() < 9) {
        std::cout << "packet to short: " << data.length() << std::endl;
        return false;
    }
    uint32_t client_id = *((uint32_t*)(data.data() + 5)) >> 24;
    if (clients.count(client_id) != 1) {
        std::cout << "invalid client_id: " << std::hex << client_id << std::dec << std::endl;
        return false;
    }
    UdpClient &client = clients[client_id];
    if (client.address != datagram.senderAddress() || client.port != datagram.senderPort()) {
        std::cout << "wrong client_id" << std::endl;
        return false;
    }
    return true;
}


void UdpServer::handle_udp()
{
    while(socket->hasPendingDatagrams())
    {
        std::cout << "============ handle_udp =============" << std::endl;
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray data = datagram.data();

        if (!check_client_id(datagram)) {
            continue;
        }

        switch (data[0]) {
        case 'A':
            std::cout << "received A" << std::endl;
            handle_ack(datagram);
            break;
        case 'P':
            std::cout << "received P" << std::endl;
            handle_position_packet(datagram);
            break;
        case 'B':
            std::cout << "received B" << std::endl;
            send_ack(datagram);
            handle_bullet_packet(datagram);
            break;
        default:
            std::cout << "received unknown packet: " << data[0] << std::endl;
        }
    }
}

void UdpServer::tick()
{
    std::cout << "============ tick =============" << std::endl;
    //send_collision(clients[42], 12, 21);
    for (auto& i: clients) {
        uint32_t client_id = i.first;
        UdpClient& client = i.second;

        // resend unacknowledged messages
        for (auto j: client.ack_pending) {
            QByteArray &data = j.second;
            socket->writeDatagram(data, client.address, client.port);
        }

        // send ship positions
        for (auto& k: clients) {
            UdpClient& dest = k.second;
            send_position_or_bullet('P', dest, client.ship);
        }
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    UdpServer server;
    return app.exec();
}
