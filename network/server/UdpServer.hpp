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
#include "../light_classes/light_object.hpp"
#include "../light_classes/light_ship.hpp"

#include "ServerPhysicsEngine.hpp"
#include "../UdpPeer.hpp"

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

    signals:
    void fightEnd(int winner_id, int health_left);

private slots:
    void handle_bullet(int id, Vector3f& position, Vector3f& velocity);
    void handle_position(int id, Vector3f& position, Vector3f& x_axis, Vector3f& y_axis, Vector3f& z_axis);
    //void handle_collision(int id, int obj_id1, int obj_id2);
    void handle_ack(int id, int seq_nr);
    void tick();

private:
    UdpPeer peer;

    ServerPhysicsEngine physics_engine;

    std::map<uint32_t, UdpClient> clients;

    bool check_client_id(int id); //QNetworkDatagram &datagram);

    QTimer timer;
    QTime time;
public:
    UdpServer();
    void add_client(int id, QHostAddress addr, int port, Vector3f& pos, int health);
    ServerPhysicsEngine& get_physics_engine() { return physics_engine; }
    void start();
    void stop();
};

#endif
