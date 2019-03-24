#ifndef SERVER_H
#define SERVER_H

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>
#include <math/Vector.hpp>
#include <math/Quaternion.hpp>
#include <physics/PhysicalBullet.hpp>
#include <physics/Transformable.hpp>
#include <physics/Hittable.hpp>
#include <memory>
#include <light_object.hpp>
#include <light_ship.hpp>

#include "ServerPhysicsEngine.hpp"

using namespace asteroids;

class UdpClient
{
public:
    uint32_t id;
    QHostAddress address;
    quint16 port;
    uint32_t seq_nr = 0;
    Hittable::Ptr ship;
    std::map<uint32_t, QByteArray> ack_pending;

    UdpClient(): ship(new Hittable(0)), id(0) {}
    UdpClient(uint32_t id): ship(new Hittable(id << 24)), id(id) {}
    uint32_t next_seq_nr() { return seq_nr++; }
};

class UdpServer: public QObject
{
    Q_OBJECT

private slots:
    void handle_udp();
    void tick();

private:
    ServerPhysicsEngine physics_engine;

    std::unique_ptr<QUdpSocket> socket;
    std::unique_ptr<QTimer> timer;
    std::map<uint32_t, UdpClient> clients;

    Vector3f bytes_to_vector(char *bytes);
    Quaternion bytes_to_quaternion(char *bytes);
    void handle_position_packet(QNetworkDatagram &datagram);
    void handle_bullet_packet(QNetworkDatagram &datagram);
    void set_position_from_packet(QNetworkDatagram &datagram, Transformable &obj);
    void send_ack(QNetworkDatagram &datagram);
    void handle_ack(QNetworkDatagram &datagram);
    void send_collision(UdpClient &client, uint32_t id1, uint32_t id2);
    void send_position(UdpClient &client, Transformable &obj, uint32_t obj_id);
    void send_bullet(UdpClient &client, PhysicalBullet &obj, uint32_t obj_id);
    bool check_client_id(QNetworkDatagram &datagram);

    QTime time;
public:
    UdpServer();
    void add_client(int id, QHostAddress addr);
    void start();
};

#endif
