#include "UdpServer.hpp"

#include <QNetworkDatagram>
#include <iostream>
#include <thread>
#include <memory>
#include <chrono>
#include <light_bullet.hpp>

using namespace asteroids;

UdpServer::UdpServer()
{
    socket = std::unique_ptr<QUdpSocket>(new QUdpSocket(nullptr));

    if (!socket->bind(QHostAddress(QString("0.0.0.0")), 1235)) {
        std::cout << "Failed to bind socket" << std::endl;
    }
    connect(socket.get(), &QUdpSocket::readyRead, this, &UdpServer::handle_udp);

    timer = std::unique_ptr<QTimer>(new QTimer(nullptr));
    connect(timer.get(), &QTimer::timeout, this, &UdpServer::tick);
    //timer->start(1000/30);


    /*
    clients[42] = UdpClient(42);
    clients[42].address = QHostAddress::LocalHost;
    clients[42].port = 1234;
    clients[42].ship->setHealth(10);

    clients[43] = UdpClient(43);
    clients[43].address = QHostAddress(QString("192.168.0.43"));
    clients[43].port = 1234;
    clients[43].ship->setHealth(10);
    
    physics_engine.addHittable(clients[42].ship);
    physics_engine.addHittable(clients[43].ship);
    */

    PhysicalObject::Ptr a(new PhysicalObject(Vector3f(), Vector3f(1000, 0, 0), 0, 0, 0, 0, 100, 1));
    PhysicalObject::Ptr b(new PhysicalObject(Vector3f(), Vector3f(-1000, 0, 0), 0, 0, 0, 0, 100, 2));
    PhysicalObject::Ptr c(new PhysicalObject(Vector3f(), Vector3f(0, 1000, 0), 0, 0, 0, 0, 100, 3));
    PhysicalObject::Ptr d(new PhysicalObject(Vector3f(), Vector3f(0, -1000, 0), 0, 0, 0, 0, 100, 4));
    physics_engine.addDestroyable(a);
    physics_engine.addDestroyable(b);
    physics_engine.addDestroyable(c);
    physics_engine.addDestroyable(d);

    time.start();
}



asteroids::Vector3f UdpServer::bytes_to_vector(char *bytes)
{
    asteroids::Vector3f vec;
    for (int i = 0; i < 3; i++) {
        vec[i] = *((float *)(bytes + i * 4));
    }
    return vec;
}


void UdpServer::set_position_from_packet(QNetworkDatagram &datagram, Transformable &obj)
{
    //std::cout << "received position data:" << std::endl;
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
    obj.setPosition(position);
    //obj.setVelocity(velocity);
    obj.setXAxis(x_axis);
    obj.setYAxis(y_axis);
    obj.setZAxis(z_axis);
    // std::cout << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //           << " v: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl
    //           << " x: " << x_axis[0] << ", " <<  x_axis[1] << ", " <<  x_axis[2] << std::endl
    //           << " y: " << y_axis[0] << ", " <<  y_axis[1] << ", " <<  y_axis[2] << std::endl
    //           << " z: " << z_axis[0] << ", " <<  z_axis[1] << ", " <<  z_axis[2] << std::endl;

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

    set_position_from_packet(datagram, *clients[client_id].ship);
}

void UdpServer::handle_bullet_packet(QNetworkDatagram &datagram)
{
    QByteArray data = datagram.data();
    if (data.length() < 9 + 6 * 4) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    uint32_t obj_id = *((int32_t*)(data.data() + 5));
    uint32_t client_id = obj_id >> 24;
    // TODO use positons

    asteroids::Vector3f position = bytes_to_vector(data.data() + 9);
    asteroids::Vector3f velocity = bytes_to_vector(data.data() + 9 + 3 * 4);

    PhysicalBullet::Ptr bullet(new PhysicalBullet(position, velocity, client_id << 24, obj_id));

    physics_engine.addBullet(bullet);
    //std::cout << "created bullet: " << obj_id << std::endl;


    //std::cout << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //          << " v: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl;

    for (auto& i: clients) {
        UdpClient& dest = i.second;
        if (dest.id != client_id) {
            send_bullet(dest, *bullet, obj_id);
        }
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
    //std::cout << "sending ACK seq_nr: " << seq_nr << std::endl;
    QByteArray reply_data;
    reply_data.append('A');
    reply_data.append((char *)(&seq_nr), 4);

    socket->writeDatagram(datagram.makeReply(reply_data));
}

void UdpServer::send_collision(UdpClient &client, uint32_t obj_id1, uint32_t obj_id2)
{
    std::cout << "sending col" << std::endl;
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

void UdpServer::send_position(UdpClient &client, Transformable& obj, uint32_t obj_id)
{
    uint32_t seq_nr = client.next_seq_nr();
    //std::cout << "sending  " << 'P' << " to " << client.id << " seq_nr " << seq_nr << std::endl;
    QByteArray data;

    data.append('P');
    data.append((char *)(&seq_nr), 4);
    data.append((char *)(&obj_id), 4);

    asteroids::Vector3f position = obj.getPosition();
    asteroids::Vector3f velocity(); //obj.getVelocity();
    asteroids::Vector3f x_axis = obj.getXAxis();


    asteroids::Vector3f y_axis = obj.getYAxis();
    asteroids::Vector3f z_axis = obj.getZAxis();


    data.append((char *)(&position[0]), 4);
    data.append((char *)(&position[1]), 4);
    data.append((char *)(&position[2]), 4);

    for (int i = 0; i < 12; i++) {
        data.append('\0');
    }

    data.append((char *)(&x_axis[0]), 4);
    data.append((char *)(&x_axis[1]), 4);
    data.append((char *)(&x_axis[2]), 4);

    data.append((char *)(&y_axis[0]), 4);
    data.append((char *)(&y_axis[1]), 4);
    data.append((char *)(&y_axis[2]), 4);

    data.append((char *)(&z_axis[0]), 4);
    data.append((char *)(&z_axis[1]), 4);
    data.append((char *)(&z_axis[2]), 4);


    // std::cout << " senging position: " << std::endl
    //           << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //           << " x: " << x_axis[0] << ", " <<  x_axis[1] << ", " <<  x_axis[2] << std::endl
    //           << " y: " << y_axis[0] << ", " <<  y_axis[1] << ", " <<  y_axis[2] << std::endl
    //           << " z: " << z_axis[0] << ", " <<  z_axis[1] << ", " <<  z_axis[2] << std::endl;

    socket->writeDatagram(data, client.address, client.port);}

void UdpServer::send_bullet(UdpClient &client, PhysicalBullet& obj, uint32_t obj_id)
{
    uint32_t seq_nr = client.next_seq_nr();
    //std::cout << "sending  " << 'B' << " to " << client.id << " seq_nr " << seq_nr << std::endl;
    QByteArray data;

    data.append('B');
    data.append((char *)(&seq_nr), 4);
    data.append((char *)(&obj_id), 4);

    asteroids::Vector3f position = obj.getPosition();
    asteroids::Vector3f velocity = obj.direction();


    data.append((char *)(&position[0]), 4);
    data.append((char *)(&position[1]), 4);
    data.append((char *)(&position[2]), 4);

    data.append((char *)(&velocity[0]), 4);
    data.append((char *)(&velocity[1]), 4);
    data.append((char *)(&velocity[2]), 4);


    // std::cout << " senging position: " << std::endl
    //           << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //           << " x: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl;

    socket->writeDatagram(data, client.address, client.port);
    client.ack_pending[seq_nr] = data;
}

void UdpServer::handle_ack(QNetworkDatagram &datagram)
{
    std::cout << "handle_ack" << std::endl;
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
    // if (client.address != datagram.senderAddress() || client.port != datagram.senderPort()) {
    //     qDebug() << datagram.senderAddress();
    //     std::cout << "wrong client_id" << std::endl;
    //     return false;
    // }
    return true;
}


void UdpServer::handle_udp()
{
    while(socket->hasPendingDatagrams())
    {
        //std::cout << "============ handle_udp =============" << std::endl;
        QNetworkDatagram datagram = socket->receiveDatagram();
        QByteArray data = datagram.data();

        if (!check_client_id(datagram)) {
            continue;
        }

        switch (data[0]) {
        case 'A':
            //std::cout << "received A" << std::endl;
            handle_ack(datagram);
            break;
        case 'P':
            //std::cout << "received P" << std::endl;
            handle_position_packet(datagram);
            break;
        case 'B':
            //std::cout << "received B" << std::endl;
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
    //std::cout << "============ tick =============" << std::endl;
    int time_elapsed = time.restart();
    bool game_over = physics_engine.process(time_elapsed);
    auto collisions = physics_engine.detect_collisions();
    for (auto& i: clients) {

        uint32_t client_id = i.first;
        UdpClient& client = i.second;



        // resend unacknowledged messageserasing client
        if (client.ack_pending.size() > 0) {
            std::cout << "resending " << client.ack_pending.size() << " packets to " << client.id << std::endl;
        }
        if (client.ack_pending.size() > 100) {
            std::cout  << client.ack_pending.size() << " unacknowledged packets. giving up on " << client.id << std::endl;
            break;
        }

        for (auto i: collisions) {
            std::cout << "COL2: " << i.first << " " << i.second << endl;
            send_collision(client, i.first, i.second);
        }

        for (auto j: client.ack_pending) {
            QByteArray &data = j.second;
            //socket->writeDatagram(data, client.address, client.port);
        }

        // send ship positions
        for (auto& k: clients) {
            UdpClient& dest = k.second;
            if (client.id != dest.id) {
                send_position(dest, *client.ship, client.id << 24);
            }
        }
    }
    if (game_over) {
        std::cout << "GAME OVER!!!!!!!!!!!!!!!" << std::endl;
        timer->stop();
    }
}


void UdpServer::start()
{
    timer->start(1000/30);
}

void UdpServer::add_client(int id, QHostAddress addr)
{

    clients[id] = UdpClient(id);
    clients[id].address = addr;
    clients[id].port = 1234;
    clients[id].ship->setHealth(10);

    physics_engine.addHittable(clients[id].ship);
}
