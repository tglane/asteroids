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
    std::cout << "send_init" << std::endl;
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

void tcpclient::endround_slot()
{
    std::cout << "endround_slot" << std::endl;
    QJsonArray init_array;
    init_array.push_back("state");
    init_array.push_back(m_datamodel->createJson(m_datamodel->getSelfPlayer()));

    QJsonDocument doc(init_array);

    int size = doc.toJson().size();
    m_socket->write((char*) &size, sizeof(size));

    m_socket->write(doc.toJson());
    m_socket->flush();

    m_state = client_state::END_ROUND;
}

void tcpclient::recv_json()
{
    if(last_size == 0)
    {
        m_socket->read((char*) &last_size, 4);
    }

    if(m_socket->bytesAvailable() == last_size)
    {
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
            process_fight_init(recv_array[1].toObject());
        }
        else
        {
            std::cout << m_state << " | " << recv_array[0].toString().toStdString() << std::endl;
        }

        last_size = 0;
    }

}

void tcpclient::process_init_res(QJsonObject recv_obj)
{
    m_player_id = recv_obj["id"].toInt();
    m_datamodel->getUniverse(recv_obj["map"].toString().toStdString());
    m_datamodel->constructPlayer(m_player_id,
                                 m_player_name.toStdString(), true);



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

void tcpclient::process_strat_init(QJsonArray recv_array) {
    //m_datamodel->getSelfPlayer()->setPlayerName(m_player_name.toStdString());

    std::cout << "strat_init" << std::endl;

    for (int i = 1; i < recv_array.size(); i++) {
        //std::cout << "id: " << recv_array[i].toObject()["id"].toInt() << "m_plyer_id: " << m_player_id << std::endl;
        bool is_self = recv_array[i].toObject()["id"].toInt() == m_player_id;
        m_datamodel->constructPlayer(recv_array[i].toObject()["id"].toInt(),
                                     recv_array[i].toObject()["player_name"].toString().toStdString(), is_self);

    }
    emit start_round();
    m_state = client_state::ROUND;

    //send_ready();
    //m_state = client_state::END_ROUND;
}

void tcpclient::process_state(QJsonArray recv_array)
{
    QJsonObject obj = recv_array[1].toObject();
    m_datamodel->updateAll(obj);

    obj = recv_array[2].toObject();
    m_datamodel->updateAll(obj);

    m_state = client_state::ROUND;

    qDebug() << m_datamodel->createJson(m_datamodel->getSelfPlayer());
    qDebug() << m_datamodel->createJson(m_datamodel->getEnemyPlayer());
}

void tcpclient::process_fight_init(QJsonObject recv_obj)
{
    /* Initialize the udp client for the connection during 3d fight */
    m_udpclient = std::make_shared<udpclient>(m_player_id, m_server_ip, m_socket->localPort());
    m_udpclient->init_fight_slot(recv_obj);
    std::cout << "fight init" << std::endl;

    /* Initialize new window for 3d part */
    m_mainwindow = std::make_shared<asteroids::MainWindow>("../models/level.xml");
    //m_mainwindow->showFullScreen();
    m_mainwindow->show();
    m_physicsEngine = m_mainwindow->ui->openGLWidget->getPhysicsEngine();

    /* Parse fight_init package */
    QJsonArray asteroids_arr = recv_obj["asteroids"].toArray();
    for(auto asteroid_value : asteroids_arr)
    {
        QJsonObject asteroid_object = asteroid_value.toObject();
        asteroids::Vector3f pos(asteroid_object["position_x"].toDouble(), asteroid_object["position_y"].toDouble(), asteroid_object["position_z"].toDouble());
        asteroids::Vector3f xAxis(asteroid_object["x_axis_x"].toDouble(), asteroid_object["x_axis_y"].toDouble(), asteroid_object["x_axis_z"].toDouble());
        asteroids::Vector3f yAxis(asteroid_object["y_axis_x"].toDouble(), asteroid_object["y_axis_y"].toDouble(), asteroid_object["y_axis_z"].toDouble());
        asteroids::Vector3f zAxis(asteroid_object["z_axis_x"].toDouble(), asteroid_object["z_axis_y"].toDouble(), asteroid_object["z_axis_z"].toDouble());
        float speed = asteroid_object["speed"].toDouble();
        float radius = asteroid_object["radius"].toDouble();
        int id = asteroid_object["id"].toInt();

        //TODO filename for asteroid texture?
        TexturedMesh::Ptr mesh = std::static_pointer_cast<TexturedMesh>(TriangleMeshFactory::instance().getMesh("../models/asteroid.3ds"));
        Asteroid::Ptr asteroid = std::make_shared<Asteroid>(mesh, Vector3f(), pos, 0, 0, speed, 0, radius, id);
        asteroid->setXAxis(xAxis);
        asteroid->setYAxis(yAxis);
        asteroid->setZAxis(zAxis);
        m_physicsEngine->addDestroyable(asteroid);
    }

    QJsonArray player_arr = recv_obj["players"].toArray();
    for(auto player_value : player_arr)
    {
        QJsonObject player_object = player_value.toObject();
        asteroids::Vector3f pos(player_object["position_x"].toDouble(), player_object["position_y"].toDouble(), player_object["position_z"].toDouble());
        asteroids::Vector3f xAxis(player_object["x_axis_x"].toDouble(), player_object["x_axis_y"].toDouble(), player_object["x_axis_z"].toDouble());
        asteroids::Vector3f yAxis(player_object["y_axis_x"].toDouble(), player_object["y_axis_y"].toDouble(), player_object["y_axis_z"].toDouble());
        asteroids::Vector3f zAxis(player_object["z_axis_x"].toDouble(), player_object["z_axis_y"].toDouble(), player_object["z_axis_z"].toDouble());
        int ship_count = player_object["ship_count"].toInt();

        if(player_object["id"].toInt() == m_player_id)
        {
            m_mainwindow->ui->openGLWidget->getCamera()->setPosition(pos);
            m_mainwindow->ui->openGLWidget->getCamera()->setXAxis(xAxis);
            m_mainwindow->ui->openGLWidget->getCamera()->setYAxis(yAxis);
            m_mainwindow->ui->openGLWidget->getCamera()->setZAxis(zAxis);

            m_mainwindow->ui->openGLWidget->getCamera()->setHealth(ship_count * 10);
        }
        else
        {
            m_mainwindow->ui->openGLWidget->getEnemy()->setPosition(pos);
            m_mainwindow->ui->openGLWidget->getEnemy()->setXAxis(xAxis);
            m_mainwindow->ui->openGLWidget->getEnemy()->setYAxis(yAxis);
            m_mainwindow->ui->openGLWidget->getEnemy()->setZAxis(zAxis);

            m_mainwindow->ui->openGLWidget->getEnemy()->setHealth(ship_count * 10);
        }
    }

    m_mainwindow->ui->openGLWidget->setClient(m_udpclient);
}
