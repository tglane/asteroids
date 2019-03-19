#ifndef SERVER_H
#define SERVER_H

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <math/Vector.hpp>
#include <memory>
#include <light_object.hpp>

class Client
{
public:
    QHostAddress address;
    quint16 port;
    uint32_t seq_nr = 0;
    //std::shared_ptr<light_ship> ship;
    std::map<uint32_t, QByteArray> ack_pending;

    uint32_t next_seq_nr() { return seq_nr++; }
};

class Server: public QObject
{
    Q_OBJECT

private slots:
    void handle_udp();

private:
    std::map<uint32_t, std::unique_ptr<light_object>> objects;
    std::unique_ptr<QUdpSocket> socket;
    std::map<uint32_t, Client> clients;

    asteroids::Vector3f bytes_to_vector(char *bytes);
    void handle_position_packet(QNetworkDatagram &datagram);
    void handle_bullet_packet(QNetworkDatagram &datagram);
    void send_ack(QNetworkDatagram &datagram);
    void handle_ack(QNetworkDatagram &datagram);
    void send_collision(Client &client, uint32_t id1, uint32_t id2);
    bool check_client_id(QNetworkDatagram &datagram);

public:
    Server();
    void run();

};

#endif
