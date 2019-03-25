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
#include "datamodel/DataModel.hpp"
#include "udpclient.hpp"

class tcpclient : public QObject {

    Q_OBJECT

    enum client_state { CONNECTING, WAIT, READY, ROUND, END_ROUND, FIGHT };

public:

    using Ptr = std::shared_ptr<tcpclient>;

    tcpclient(asteroids::DataModel::Ptr datamodel, QString player_name, QString server_ip, QObject* parent = 0);

    void connect_to_server();

public slots:
    void send_ready();

private slots:
    void send_init();

    void recv_json();

    void error_out(QAbstractSocket::SocketError) { std::cout << "error" << std::endl; }

signals:
    void fight_init_signal(QJsonObject);

private:

    /// receive id (and map/level name)
    void process_init_res(QJsonObject recv_obj);

    /// receive player list
    void process_strat_init(QJsonArray recv_array);

    /// receive new data state after a strat round
    void process_state(QJsonArray recv_array);

    client_state m_state; /// Represents tcp client state

    QString m_player_name;

    QString m_server_ip;

    asteroids::DataModel::Ptr m_datamodel;

    udpclient::Ptr m_udpclient;

    std::shared_ptr<QTcpSocket> m_socket;

};


#endif //CPP18_ASTEROIDS_TCPCLIENT_HPP
