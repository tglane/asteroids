//
// Created by timog on 22.03.19.
//

#include <QDebug>
#include "tcpclient.hpp"
#include "view/MainWindow.hpp"

tcpclient::tcpclient(asteroids::DataModel::Ptr datamodel, QObject* parent)
    : m_datamodel(std::move(datamodel))
{
    m_socket = std::make_shared<QTcpSocket>(this);
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    connect(m_socket.get(), SIGNAL(connected()), this, SLOT(send_init()));
    connect(m_socket.get(), SIGNAL(readyRead()), this, SLOT(recv_json()));

    connect_to_server("ASDF", "192.168.0.42");
}

void tcpclient::connect_to_server(string name, string server_ip)
{
    m_player_name = QString::fromStdString(name);
    m_server_ip = QString::fromStdString(server_ip);

    m_socket->connectToHost(m_server_ip, 1235);
}

void tcpclient::send_ready()
{
    std::cout << "send_ready" << std::endl;
    QJsonArray init_array;
    init_array.push_back("ready");
    //init_array.push_back(m_datamodel->createJson(m_datamodel->getSelfPlayer()));

    QJsonDocument doc(init_array);

    int size = doc.toJson().size();
    m_socket->write((char*) &size, sizeof(size));

    m_socket->write(doc.toJson());
    m_socket->flush();
}

void tcpclient::send_init()
{
    std::cout << "afsdfgn" << std::endl;
    QJsonObject init_object;
    init_object.insert("player_name", QJsonValue::fromVariant(m_player_name));

    QJsonArray init_array;
    init_array.push_back("init");
    init_array.push_back(init_object);

    QJsonDocument doc(init_array);

    int size = doc.toJson().size();
    m_socket->write((char*) &size, sizeof(size));

    m_socket->write(doc.toJson());
    m_socket->flush();

    m_state = client_state::CONNECTING;
}

void tcpclient::recv_json()
{
    int size;
    m_socket->read((char*) &size, 4);

    QByteArray recv_data(m_socket->readAll());
    QString recv_json(recv_data);
    QJsonDocument json_doc = QJsonDocument::fromJson(recv_json.toUtf8());
    QJsonArray recv_array = json_doc.array();

    if(recv_array[0] == "init_res" && m_state == client_state::CONNECTING)
    {
        process_init_res(recv_array[1].toObject());
    }
    else if(recv_array[0] == "strat_init" && m_state == client_state::READY)
    {
        process_strat_init(recv_array);
    }
    else if(recv_array[0] == "state" && (m_state == client_state::END_ROUND || m_state == client_state::FIGHT))
    {
        process_state(recv_array);
    }
    else if (recv_array[0] == "fight_init" && (m_state == client_state::END_ROUND || m_state == client_state::FIGHT) )
    {
        m_udpclient = std::shared_ptr<udpclient>(new udpclient(m_datamodel->getSelfPlayer()->getIdentity(), m_server_ip));
        m_udpclient->init_fight_slot(recv_array[2].toObject());
        std::cout << "fight iniz" << std::endl;

        m_mainwindow = std::make_shared<asteroids::MainWindow>("../models/level.xml");
        //m_mainwindow->showFullScreen();
        m_mainwindow->show();
        m_mainwindow->ui->openGLWidget->setClient(m_udpclient);
    }
    else
    {
        std::cout << m_state << " | " << recv_array[0].toString().toStdString() << std::endl;
    }
}

void tcpclient::process_init_res(QJsonObject recv_obj)
{
    m_datamodel->setOwnID(recv_obj["id"].toInt());
    m_datamodel->getUniverse(recv_obj["map"].toString().toStdString());

    //m_datamodel->switchWindow(asteroids::DataModel::MAIN2D);

    m_state = client_state::WAIT;

    QJsonArray init_array;
    init_array.push_back("ready");

    QJsonDocument doc(init_array);

    int size = doc.toJson().size();
    m_socket->write((char*) &size, sizeof(size));

    m_socket->write(doc.toJson());
    m_socket->flush();

    m_state = client_state::READY;

    send_ready();
}

void tcpclient::process_strat_init(QJsonArray recv_array)
{
    m_datamodel->getSelfPlayer()->setPlayerName(m_player_name.toStdString());

    std::cout << "strat_init" << std::endl;

    for(int i = 1; i < recv_array.size(); i++)
    {
        if(recv_array[i].toObject()["id"] != m_datamodel->getSelfPlayer()->getIdentity())
        {
            /* Sets enemy player name to the name sent from server */
            m_datamodel->getEnemyPlayer(recv_array[i].toObject()["id"].toInt())->setPlayerName(recv_array[i].toObject()["player_name"].toString().toStdString());
        }
    }

    m_state = client_state::ROUND;

    send_ready();
    m_state = client_state::END_ROUND;
}

void tcpclient::process_state(QJsonArray recv_array)
{
    QJsonDocument doc;
    doc.setObject(recv_array[1].toObject());
    //m_datamodel->updateAll(doc);

    //TODO maybe not correct ---
    doc.setObject(recv_array[2].toObject());
    //m_datamodel->updateAll(doc);
}
