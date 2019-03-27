#include "TcpServer.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QApplication>

#include <util/AsteroidField.hpp>
TcpServer::TcpServer()
    : server(this)
{
    state = WAITING;
    connect(&server, SIGNAL(newConnection()), this, SLOT(onConnect()));
    //connect(&server, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    m_datamodel = DataModel::Ptr(new DataModel());
    m_datamodel->getUniverse("../resources/Level-1.map");

    if(!server.listen(QHostAddress::Any, 1235))
    {
        qDebug() << "Error: " << server.errorString();
    }
    else
    {
        qDebug() << "Server started ";

    }
}

void TcpServer::onDisconnect()
{
    qDebug() << "Client disconnected";
}

void TcpServer::onConnect()
{
    qDebug() << "Client connected";
    if (state != WAITING) {
        return; 
    }
    qDebug() << "Client connected 2";

    TcpClient client(last_id++, server.nextPendingConnection());
    client.socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    clients.push_back(client);

    connect(client.socket.get(), SIGNAL(disconnect()), this, SLOT(onDisconnect()));
    connect(client.socket.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void TcpServer::send_state()
{
    for (auto client: clients)
    {
        qDebug() << "Sending state";
        QJsonArray array;
        array.push_back("state");
        for (auto j: clients) {
            Player::Ptr temp_player = m_datamodel->getPlayer(j.id);
            array.push_back(m_datamodel->createJsonPlayerStatus(temp_player));
        }
        QJsonDocument doc(array);
        int size = doc.toJson().size();
        client.socket->write((char*)&size, 4);
        client.socket->write(doc.toJson());
    }
}

void TcpServer::handle_ready(TcpClient& client, QJsonDocument& doc)
{
    qDebug() << "ready received";
    qDebug() << doc;
    if (state == WAIT_READY)
    {
        ready_count++;
        if (clients.size() == ready_count) {
            for (auto j: clients) {
                QJsonArray array;
                array.push_back("strat_init");
                qDebug() << "Sending strat_init";

                for (auto i: clients)
                {
                    QJsonObject player_obj;
                    player_obj.insert("id", QJsonValue::fromVariant(client.id));
                    Player::Ptr player = m_datamodel->getPlayer(client.id);
                    player_obj.insert("player_name", QJsonValue::fromVariant(QString::fromStdString(player->getPlayerName())));

                    array.push_back(player_obj);
                }

                QJsonDocument res(array);
                int size = res.toJson().size();
                j.socket->write((char*)&size, 4);
                j.socket->write(res.toJson());
            }

            qDebug() << "state changed: ROUND";
            state = ROUND;
            ready_count = 0;
        }
    } else if (state == ROUND) {
        //TODO berechnungen / kämpfe
        ready_count++;
        handle_state(client, doc);
        if (clients.size() == ready_count) {
            //m_battle_list = m_datamodel->findBattles();
            //m_battle_list = std::vector<Battle::Ptr>{Battle::Ptr(nullptr)};

            state = FIGHT;
            battle_count = 0;
            // Relevant,
            //udpServer.start();
            send_battle();

            qDebug() << "state changed: FIGHT";
        }
    }
}

void TcpServer::handle_state(TcpClient& client, QJsonDocument& doc) {
    QJsonArray array_state = doc.array();
    QJsonObject obj_state = array_state[1].toObject();
    m_datamodel->updateAll(obj_state);
}

void TcpServer::fight_init()
{
    AsteroidField asteroids(20);
    std::list<PhysicalObject::Ptr> asteroid_list;
    asteroids.getAsteroids(asteroid_list);

    qDebug() << "sending fight_init";
    udpServer = std::shared_ptr<UdpServer>(new UdpServer());
    connect(udpServer.get(),SIGNAL(fightEnd(int,int)), this, SLOT(fightEnd(int,int)));

    for (auto i: asteroid_list) {
        udpServer->get_physics_engine().addDestroyable(i);
    }
    for (auto j: clients)
    {
        Vector3f position;
        if (j.id % 2 == 0) {
            position = Vector3f(-2500, 0, 0);
        } else {
            position = Vector3f(2500, 0, 0);
        }

        udpServer->add_client(j.id, j.socket->peerAddress(), j.socket->peerPort(), position, 10);
        QJsonArray array;
        array.push_back("fight_init");

        QJsonArray asteroid_array;
        for (auto i: asteroid_list) {
            QJsonObject asteroid_obj;
            Vector3f position = i->getPosition();
            Vector3f x_axis = i->getXAxis();
            Vector3f y_axis = i->getYAxis();
            Vector3f z_axis = i->getZAxis();

            asteroid_obj.insert("position_x", QJsonValue::fromVariant(position[0]));
            asteroid_obj.insert("position_y", QJsonValue::fromVariant(position[1]));
            asteroid_obj.insert("position_z", QJsonValue::fromVariant(position[2]));
            asteroid_obj.insert("x_axis_x", QJsonValue::fromVariant(x_axis[0]));
            asteroid_obj.insert("x_axis_y", QJsonValue::fromVariant(x_axis[1]));
            asteroid_obj.insert("x_axis_z", QJsonValue::fromVariant(x_axis[2]));
            asteroid_obj.insert("y_axis_x", QJsonValue::fromVariant(y_axis[0]));
            asteroid_obj.insert("y_axis_y", QJsonValue::fromVariant(y_axis[1]));
            asteroid_obj.insert("y_axis_z", QJsonValue::fromVariant(y_axis[2]));
            asteroid_obj.insert("z_axis_x", QJsonValue::fromVariant(z_axis[0]));
            asteroid_obj.insert("z_axis_y", QJsonValue::fromVariant(z_axis[1]));
            asteroid_obj.insert("z_axis_z", QJsonValue::fromVariant(z_axis[2]));
            asteroid_obj.insert("speed", QJsonValue::fromVariant(i->getSpeed()));
            asteroid_obj.insert("radius", QJsonValue::fromVariant(i->radius()));
            asteroid_obj.insert("id", QJsonValue::fromVariant(i->get_id()));

            asteroid_array.push_back(asteroid_obj);
        }

        QJsonArray player_array;
        for (auto i: clients) {
            Vector3f position;
            Vector3f x_axis;
            Vector3f y_axis;
            if (i.id % 2 == 0) {
                position = Vector3f(-1000, 0, 0);
                x_axis = Vector3f(1, 0, 0);
                 y_axis = Vector3f(0, 1, 0);
            } else {
                position = Vector3f(1000, 0, 0);
                x_axis = Vector3f(-1, 0, 0);
                y_axis = Vector3f(0, -1, 0);
            }
            QJsonObject player_obj;
            Vector3f z_axis = Vector3f(0, 0, 1);
            player_obj.insert("position_x", QJsonValue::fromVariant(position[0]));
            player_obj.insert("position_y", QJsonValue::fromVariant(position[1]));
            player_obj.insert("position_z", QJsonValue::fromVariant(position[2]));
            player_obj.insert("x_axis_x", QJsonValue::fromVariant(x_axis[0]));
            player_obj.insert("x_axis_y", QJsonValue::fromVariant(x_axis[1]));
            player_obj.insert("x_axis_z", QJsonValue::fromVariant(x_axis[2]));
            player_obj.insert("y_axis_x", QJsonValue::fromVariant(y_axis[0]));
            player_obj.insert("y_axis_y", QJsonValue::fromVariant(y_axis[1]));
            player_obj.insert("y_axis_z", QJsonValue::fromVariant(y_axis[2]));
            player_obj.insert("z_axis_x", QJsonValue::fromVariant(z_axis[0]));
            player_obj.insert("z_axis_y", QJsonValue::fromVariant(z_axis[1]));
            player_obj.insert("z_axis_z", QJsonValue::fromVariant(z_axis[2]));
            player_obj.insert("id", QJsonValue::fromVariant(i.id));
            player_array.push_back(player_obj);
        }


        QJsonObject obj;
        obj.insert("asteroids", asteroid_array);
        obj.insert("players", player_array);
        array.push_back(obj);

        QJsonDocument res(array);
        qDebug() << res;
        int size = res.toJson().size();
        j.socket->write((char*)&size, 4);
        j.socket->write(res.toJson());
    }

    udpServer->start();
}

void TcpServer::fightEnd(int id, int health_left) {
    // ToDo Datamodel
    Battle::Ptr current_battle = m_battle_list[battle_count];
    if (current_battle->m_player1->getIdentity() == id) {
        current_battle->m_player1->delShips(current_battle->m_numberShips1 - health_left);
    } else {
        current_battle->m_player1->delShips(current_battle->m_numberShips1);
    }
    if (current_battle->m_player2->getIdentity() == id) {
        current_battle->m_player2->delShips(current_battle->m_numberShips2 - health_left);
    } else {
        current_battle->m_player2->delShips(current_battle->m_numberShips2);
    }

    m_datamodel->getPlayer(clients[id].id)->addPlanet(m_battle_list[battle_count]->m_location);

    current_battle->m_location->setOwner(m_datamodel->getPlayer(clients[id].id));
    current_battle->m_location->setInvader(nullptr);
    current_battle->m_location->setInvaderShips(0);
    current_battle->m_location->setShips(health_left);

    udpServer.reset();
    send_battle();
}

void TcpServer::send_battle() {
    //if (battle_count < m_battle_list.size()) {
        // ToDo
        fight_init();
        battle_count++;

        /*
    } else {
        state = ROUND;
        qDebug() << "state changed: ROUND";
        send_state();
    }
        */
}

void TcpServer::handle_init(TcpClient& client, QJsonDocument& doc)
{
    qDebug() << "init received";
    qDebug() << doc;

    // Ziehe Daten aus Document
    QJsonArray array = doc.array();
    QJsonObject temp_name = array[1].toObject();


    QString name(temp_name["playername"].toString().toUtf8());

    m_datamodel->getPlayer(client.id)->setPlayerName(name.toUtf8().constData());


    qDebug() << "Sending init_res";
    QJsonObject init_res;
    init_res.insert("id", QJsonValue::fromVariant(client.id));
    init_res.insert("map", QJsonValue::fromVariant("../resources/Level-1.map"));

    QJsonArray array_response;
    array_response.push_back("init_res");
    array_response.push_back(init_res);
    QJsonDocument res(array_response);

    int size = res.toJson().size();
    client.socket->write((char*)&size, 4);
    client.socket->write(res.toJson());

    init_count++;
    qDebug() << init_count;
    if (init_count == 2) {
        qDebug() << "state changed: WAIT_READY";
        state = WAIT_READY;
    }
    qDebug() << client.socket->peerAddress();

    //udpServer->add_client(client.id, client.socket->peerAddress(), client.socket->peerPort(), 100);
}


void TcpServer::readyRead()
{
    for (auto i: clients)
    {
        if (i.socket->bytesAvailable() > 0)
        {
            int size;
            i.socket->read((char*) &size, 4);
            QByteArray data = i.socket->read(size);
            qDebug() << data;
            QJsonDocument doc = QJsonDocument::fromJson(QString(data).toUtf8());
            QJsonArray array = doc.array();
            QString ident(array[0].toString().toUtf8());
            if (ident == "init") {
                handle_init(i, doc);
            } else if (ident == "ready") {
                handle_ready(i, doc);
            }
        }
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TcpServer server;
    return app.exec();
}
