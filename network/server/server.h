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
    QHostAddress address;
    quint16 port;

    //std::shared_ptr<light_ship> ship;
    std::vector<QNetworkDatagram> ack_pending;
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

public:
    Server();
    void run();

};

#endif
