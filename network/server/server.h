#ifndef SERVER_H
#define SERVER_H

#include <QUdpSocket>
#include <memory>
#include <light_object.hpp>

class Client
{
    QHostAddress address;
    quint16 port;
    //std::shared_ptr<light_ship> ship;
};

class Server
{
    std::vector<light_object> objects;
    std::unique_ptr<QUdpSocket> socket;
    std::vector<Client> clients;

public:
    Server();
    void run();

};

#endif
