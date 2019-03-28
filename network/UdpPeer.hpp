#ifndef UDP_PEER_HPP
#define UDP_PEER_HPP

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

#include <math/Vector.hpp>
#include <physics/Hittable.hpp>
#include <physics/PhysicalBullet.hpp>

#include "PhysicalMissile.hpp"

namespace asteroids
{
    class UdpPeer: public QObject
    {
        Q_OBJECT

    signals:
        void received_bullet(int id, Vector3f& position, Vector3f& velocity);
        void received_position(int id, Vector3f& position, Vector3f& x_axis, Vector3f& y_axis, Vector3f& z_axis);
        void received_collision(int id, int obj_id1, int obj_id2);
        void received_ack(int id, int seq_nr);
        void received_missile(int id, Vector3f& position, Vector3f& x, Vector3f& y, Vector3f& z);
    private slots:
        void handle_udp();

    public:
        UdpPeer(QHostAddress bind_addr, int bind_port);
        UdpPeer();

        QByteArray send_position(QHostAddress& addr, int port, int seq_nr, Hittable& obj);

        QByteArray send_bullet(QHostAddress& addr, int port, int seq_nr, PhysicalBullet& bullet);

        QByteArray send_missile(QHostAddress& addr, int port, int seq_nr, PhysicalMissile& missile);

        QByteArray send_collision(QHostAddress& addr, int port, int seq_nr, int id, int obj_id1, int obj_id2);

        QByteArray send_data(QHostAddress& addr, int port, QByteArray data);

    private:
        QUdpSocket socket;

        void handle_ack(QNetworkDatagram& data);
        void handle_position_packet(QNetworkDatagram& datagram);
        void handle_bullet_packet(QNetworkDatagram& datagram);
        void handle_missile_packet(QNetworkDatagram& datagram);
        void handle_collision_packet(QNetworkDatagram& datagram);
        void send_ack(QNetworkDatagram &datagram);
        Vector3f bytes_to_vector(char *bytes);

    };
}

#endif
