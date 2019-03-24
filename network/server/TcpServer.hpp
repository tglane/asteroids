
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "UdpServer.hpp"

#include <vector>
#include <memory>

class TcpClient
{
public:
    char id;
    std::shared_ptr<QTcpSocket> socket;

    TcpClient(char id, QTcpSocket* socket)
        : id(id), socket(socket) {}

    TcpClient(const TcpClient& other)
        : id(other.id), socket(other.socket) {}
};

class TcpServer: public QObject
{
    Q_OBJECT

    enum ServerState {WAITING, WAIT_READY, ROUND, FIGHT};

private slots:
    void onConnect();
    void readyRead();

private:
    char last_id = 41;
    ServerState state;
    QTcpServer server;
    std::vector<TcpClient> clients;
    void send_strat_init();
    void send_state();

    void handle_init(TcpClient& client, QJsonDocument& doc);
    void handle_ready(TcpClient& client, QJsonDocument& doc);

    int init_count = 0;
    int ready_count = 0;

    UdpServer udpServer;

public:
    TcpServer();
};

#endif
