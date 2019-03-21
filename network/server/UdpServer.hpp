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
#include <memory>
#include <light_object.hpp>
#include <light_ship.hpp>

class UdpClient
{
public:
    uint32_t id;
    QHostAddress address;
    quint16 port;
    uint32_t seq_nr = 0;
    light_ship ship;
    std::map<uint32_t, QByteArray> ack_pending;

    UdpClient(): ship(0), id(0) {}
    UdpClient(uint32_t id): ship(id << 24), id(id) {}
    uint32_t next_seq_nr() { return seq_nr++; }
};

class UdpServer: public QObject
{
    Q_OBJECT

private slots:
    void handle_udp();
    void tick();

private:
    std::map<uint32_t, asteroids::PhysicalBullet::Ptr> bullets;
    std::unique_ptr<QUdpSocket> socket;
    std::unique_ptr<QTimer> timer;
    std::map<uint32_t, UdpClient> clients;

    asteroids::Vector3f bytes_to_vector(char *bytes);
    asteroids::Quaternion bytes_to_quaternion(char *bytes);
    void handle_position_packet(QNetworkDatagram &datagram);
    void handle_bullet_packet(QNetworkDatagram &datagram);
    void set_position_from_packet(QNetworkDatagram &datagram, asteroids::Transformable &obj);
    void send_ack(QNetworkDatagram &datagram);
    void handle_ack(QNetworkDatagram &datagram);
    void send_collision(UdpClient &client, uint32_t id1, uint32_t id2);
    void send_position_or_bullet(char type, UdpClient &client, asteroids::Transformable &obj, uint32_t obj_id);
    bool check_client_id(QNetworkDatagram &datagram);

public:
    UdpServer();
    void add_client(uint32_t id, asteroids::Vector3f position);
};

#endif
