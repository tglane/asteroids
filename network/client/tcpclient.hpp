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

/**
 * Sends, receives and parses tcp packages formated in json during the 2d strategy part.
 * Updates the datamodel of the client with the received information
 * Sends current datamodel state to the server
 */
class tcpclient : public QObject {

    Q_OBJECT

    enum client_state { CONNECTING, WAIT, READY, ROUND, END_ROUND, PRE_FIGHT, FIGHT_READY, FIGHT, END_FIGHT };

public:

    using Ptr = std::shared_ptr<tcpclient>;

    tcpclient(asteroids::DataModel::Ptr datamodel, QObject* parent = 0);

    /// Sets a pointer of a SwitchWindowInfo to set received information to the SwitchWindow
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
    /**
     * @biref signal to init a fight
     */
    void fight_init_signal(QJsonObject);

    /**
     * @brief signal to start a 2d round
     */
    void start_round();

    /**
     * @brief signal to end the blurry pause screen between the rounds
     */
    void end_pause();

    /**
     * @brief signal to stop the 2d music during a 3d fight part
     */
    void pause_music();

    /**
     * @brief signal to resume the 2d music after a 3d figth part
     */
    void resume_music();

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

    /// Size of the last received package
    int last_size = 0;

    /// Current state of the client used to handle packages right
    client_state m_state;

    /// Client name string
    QString m_player_name;

    /// Client id set by the server
    int m_player_id;

    /// Ip of the tcp server
    QString m_server_ip;

    asteroids::DataModel::Ptr m_datamodel;

    udpclient::Ptr m_udpclient;

    asteroids::PhysicsEngine::Ptr m_physicsEngine;

    std::shared_ptr<QTcpSocket> m_socket;

    std::shared_ptr<MainWindow> m_mainwindow;

    SwitchingWindowInfo* m_switch_mode_dialoge;

};

#endif //CPP18_ASTEROIDS_TCPCLIENT_HPP
