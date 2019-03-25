//
// Created by timog on 22.03.19.
//

#ifndef CPP18_ASTEROIDS_TCPCLIENT_HPP
#define CPP18_ASTEROIDS_TCPCLIENT_HPP

#include <GL/glew.h>

#include <QtCore/QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <memory>

#include "physics/PhysicsEngine.hpp"
#include "view/Camera.hpp"
#include "rendering/Asteroid.hpp"
#include "udpclient.hpp"

class tcpclient : public QObject {

    Q_OBJECT

    enum client_state { CONNECTING, READY, ROUND, END_ROUND, FIGHT };

public:

    using Ptr = std::shared_ptr<tcpclient>;

    tcpclient(QString player_name, QString server_ip, QObject* parent = 0);

    void connect_to_server(string player_name);

private slots:
    void send_init();

    void recv_json();

private:

    /// receive id (and map/level name)
    void process_init_res(QJsonObject recv_obj);

    /// receive player list
    void process_strat_init(QJsonObject recv_obj);

    /// receive new data state after a strat round
    void process_state(QJsonObject recv_obj);

    client_state m_state; /// Represents tcp client state

    QString m_player_name;

    QString m_server_ip;

    std::shared_ptr<udpclient> m_udpclient;

    std::shared_ptr<QTcpSocket> m_socket;

};


#endif //CPP18_ASTEROIDS_TCPCLIENT_HPP
