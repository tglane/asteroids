//
// Created by timog on 22.03.19.
//

#ifndef CPP18_ASTEROIDS_TCPCLIENT_HPP
#define CPP18_ASTEROIDS_TCPCLIENT_HPP

#include "util/gl_includes.h"

#include <QtCore/QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <memory>
#include <view/MainWindow.hpp>

#include "physics/PhysicsEngine.hpp"
#include "view/Camera.hpp"
#include "rendering/Asteroid.hpp"
#include "datamodel/DataModel.hpp"
#include "udpclient.hpp"
#include "view2D/SwitchingWindowInfo.hpp"

using namespace strategy;

class tcpclient : public QObject {

    Q_OBJECT

    enum client_state { CONNECTING, WAIT, READY, ROUND, END_ROUND, PRE_FIGHT, FIGHT_READY, FIGHT, END_FIGHT };

public:

    using Ptr = std::shared_ptr<tcpclient>;

    tcpclient(asteroids::DataModel::Ptr datamodel, QObject* parent = 0);

    void set_switch_pointer(SwitchingWindowInfo* sw) { m_switch_mode_dialoge = sw; }

public slots:
    /**
     * @brief Sends ready package after a round in the 2d game part
     */
    void send_ready();

    /**
     * Connects to the tcp server when the start button is pressed
     * @param name
     * @param server_ip
     */
    void connect_to_server(string name, string server_ip);

    /**
     * @brief Sends the init package to the server to receive an player id from the server
     */
    void send_init();

    /**
     * @brief Sends current game state to the server when button "next round" is pressed
     */
    void endround_slot();

private slots:
    /**
     * @brief Receives data package from server and parses the header to call certain functions
     */
    void recv_json();

signals:
    void fight_init_signal(QJsonObject);
    
    void start_round();

private:

    /// receive id (and map/level name)
    void process_init_res(QJsonObject recv_obj);

    /// receive player list
    void process_strat_init(QJsonArray recv_array);

    /// receive new data state after a strat round
    void process_state(QJsonArray recv_array);

    /// receive battle preview and battle review package
    void process_battle(QJsonObject recv_obj);

    /// receive the init package for the 3d fight
    void process_fight_init(QJsonObject recv_obj);

    int last_size = 0;

    client_state m_state; /// Represents tcp client state

    QString m_player_name;

    int m_player_id;

    QString m_server_ip;

    asteroids::DataModel::Ptr m_datamodel;

    udpclient::Ptr m_udpclient;

    asteroids::PhysicsEngine::Ptr m_physicsEngine;

    std::shared_ptr<QTcpSocket> m_socket;

    std::shared_ptr<MainWindow> m_mainwindow;

    SwitchingWindowInfo* m_switch_mode_dialoge;

};

#endif //CPP18_ASTEROIDS_TCPCLIENT_HPP
