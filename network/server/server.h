#ifndef SERVER_H
#define SERVER_H

#include <QUdpSocket>
#include <memory>

class Server
{
    std::unique_ptr<QUdpSocket> socket;

public:
    Server();
    void run();

};

#endif
