
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "UdpServer.hpp"

#include "datamodel/DataModel_Server.hpp"

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
    void onDisconnect();
    void fightEnd(int id,int  health_left);

private:
    char last_id = 42;

    ServerState state;
    QTcpServer server;
    std::vector<TcpClient> clients;

    void send_strat_init();
    void send_state();

    void fight_init();

    void handle_init(TcpClient& client, QJsonDocument& doc);
    void handle_ready(TcpClient& client, QJsonDocument& doc);
    void handle_state(TcpClient& client, QJsonDocument& doc);
    void send_battle();

    int init_count = 0;
    int ready_count = 0;
    int battle_count = 0;


    std::shared_ptr<UdpServer> udpServer;

    DataModel_Server::Ptr m_datamodel;

    std::vector<std::shared_ptr<Battle>> m_battle_list;

public:
    TcpServer();


};

#endif
