#include "UdpServer.hpp"

#include <QNetworkDatagram>
#include <iostream>
#include <thread>
#include <memory>
#include <chrono>
#include <light_bullet.hpp>

using namespace asteroids;

UdpServer::UdpServer() : peer(QHostAddress::Any, 1235)
{
    connect(&timer, &QTimer::timeout, this, &UdpServer::tick);
    connect(&peer, &UdpPeer::received_bullet, this, &UdpServer::handle_bullet);
    connect(&peer, &UdpPeer::received_position, this, &UdpServer::handle_position);
    //connect(&peer, &UdpPeer::received_collision, this, &UdpServer::handle_collision);
    connect(&peer, &UdpPeer::received_ack, this, &UdpServer::handle_ack);
    //timer.start(1000/30);


    /*clients[42] = UdpClient(42);
    clients[42].address = QHostAddress::LocalHost;
    clients[42].port = 1234;
    clients[42].ship->setHealth(10);

    clients[43] = UdpClient(43);
    clients[43].address = QHostAddress(QString("192.168.0.43"));
    clients[43].port = 1234;
    clients[43].ship->setHealth(10);

    physics_engine.addHittable(clients[42].ship);
    physics_engine.addHittable(clients[43].ship);*/


    //time.start();
}




void UdpServer::handle_position(int id, Vector3f& position, Vector3f& x_axis, Vector3f& y_axis, Vector3f& z_axis)
{
    uint32_t client_id = id >> 24;
    if (!check_client_id(id)) {
        return;
    }
    Hittable& obj = *clients[client_id].ship;
    obj.setPosition(position);
    obj.setXAxis(x_axis);
    obj.setYAxis(y_axis);
    obj.setZAxis(z_axis);
    // std::cout << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //           << " v: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl
    //           << " x: " << x_axis[0] << ", " <<  x_axis[1] << ", " <<  x_axis[2] << std::endl
    //           << " y: " << y_axis[0] << ", " <<  y_axis[1] << ", " <<  y_axis[2] << std::endl
    //           << " z: " << z_axis[0] << ", " <<  z_axis[1] << ", " <<  z_axis[2] << std::endl;

}

void UdpServer::handle_bullet(int id, Vector3f& position, Vector3f& velocity)
{
    std::cout << "handling bullet" << std::endl;
    if (!check_client_id(id)) {
        return;
    }
    uint32_t client_id = id >> 24;

    PhysicalBullet::Ptr bullet(new PhysicalBullet(position, velocity, client_id << 24, id));

    physics_engine.addBullet(bullet);
    //std::cout << "created bullet: " << obj_id << std::endl;


    //std::cout << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //          << " v: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl;

    for (auto& i: clients) {
        UdpClient& dest = i.second;
        if (dest.id != client_id) {
            peer.send_bullet(dest.address, dest.port, dest.next_seq_nr(), *bullet);
        }
    }
}


void UdpServer::handle_ack(int id, int seq_nr)
{
    std::cout << "handle_ack" << std::endl;
    if (!check_client_id(id)) {
        return;
    }
    uint32_t client_id = id >> 24;
    UdpClient &client = clients[client_id];
    if (client.ack_pending.erase(seq_nr) != 1) {
        std::cout << "invalid seq_nr" << std::endl;
    } else {
        std::cout << "seq_nr: " << seq_nr << std::endl;
    }
}

bool UdpServer::check_client_id(int id) // QNetworkDatagram &datagram)
{
    uint32_t client_id = id >> 24;
    if (clients.count(client_id) != 1) {
        std::cout << "invalid client_id: " << std::hex << client_id << std::dec << std::endl;
        return false;
    }
    //UdpClient &client = clients[client_id];
    // if (client.address != datagram.senderAddress() || client.port != datagram.senderPort()) {
    //     qDebug() << datagram.senderAddress();
    //     std::cout << "wrong client_id" << std::endl;
    //     return false;
    // }
    return true;
}


void UdpServer::tick()
{
    std::cout << "============ tick =============" << std::endl;
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
            peer.send_collision(client.address, client.port, client.next_seq_nr(), 0, i.first, i.second);
        }

        for (auto j: client.ack_pending) {
            QByteArray &data = j.second;
            peer.send_data(client.address, client.port, data);
        }

        // send ship positions
        for (auto& k: clients) {
            UdpClient& dest = k.second;
            if (client.id != dest.id) {
                peer.send_position(dest.address, dest.port, dest.next_seq_nr(), *client.ship);
            }
        }
    }
    if (game_over) {
        std::cout << "GAME OVER!!!!!!!!!!!!!!!" << std::endl;
        int winner_id = 0, remaining_health;
        for (auto cs: clients) {
            if (cs.second.ship->getHealth() > 0 ) {
                winner_id = cs.second.id;
                remaining_health = cs.second.ship->getHealth();
            }
        }
        emit fightEnd(winner_id, remaining_health);
        timer.stop();
    }
}


void UdpServer::start()
{
    timer.start(1000/30);
    time.start();
}

void UdpServer::add_client(int id, QHostAddress addr, int port, int health)
{

    clients[id] = UdpClient(id);
    clients[id].address = addr;
    clients[id].port = port;
    clients[id].ship->setHealth(health);

    physics_engine.addHittable(clients[id].ship);
}
