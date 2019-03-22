//
// Created by timog on 22.03.19.
//

#ifndef CPP18_ASTEROIDS_TCPCLIENT_HPP
#define CPP18_ASTEROIDS_TCPCLIENT_HPP

#include <GL/glew.h>

#include <QtCore/QObject>
#include <QTcpSocket>
#include <memory>

#include "physics/PhysicsEngine.hpp"
#include "view/Camera.hpp"
#include "rendering/Asteroid.hpp"
#include "udpclient.hpp"

class tcpclient : public QObject {

    Q_OBJECT

public:

    tcpclient(QString server_ip, QObject* parent = 0);

    void connect_to_server(string player_name);

private slots:
    void send_init();

private:

    enum client_state { init, init_res, ready, strat_init, state };

    QString m_server_ip;

    std::shared_ptr<udpclient> m_udpclient;

    std::shared_ptr<QTcpSocket> m_socket;

};


#endif //CPP18_ASTEROIDS_TCPCLIENT_HPP
