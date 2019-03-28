#include "TcpServer.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QApplication>

#include <util/AsteroidField.hpp>
TcpServer::TcpServer(std::string filename, QObject* parent)
{
    level = filename;
    state = WAITING;
    connect(&server, SIGNAL(newConnection()), this, SLOT(onConnect()));
    //connect(&server, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    std::cout << "../models/" << filename<<std::endl;
    m_datamodel = DataModel_Server::Ptr(new DataModel_Server("../../../models/"+filename+".map"));
    //m_datamodel->getUniverse("../../../models/Level-1.map");

    if(!server.listen(QHostAddress::Any, 1235))
    {
        qDebug() << "Error: " << server.errorString();
    }
    else
    {
        qDebug() << "Server started ";
    }

    udpServer = std::shared_ptr<UdpServer>(new UdpServer());
    connect(udpServer.get(),SIGNAL(fightEnd(int,int)), this, SLOT(fightEnd(int,int)));
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

    std::cout << "client id: " << last_id << std::endl;
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
            Player::Ptr temp_player = m_datamodel->getPlayerByID(j.id);
            array.push_back(m_datamodel->createJson(temp_player));
        }
        QJsonDocument doc(array);
        qDebug() << "sending state: " << doc;
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
            ready_count = 0;
            for (auto j: clients) {
                QJsonArray array;
                array.push_back("strat_init");
                qDebug() << "Sending strat_init";

                for (auto i: clients)
                {
                    QJsonObject player_obj;
                    player_obj.insert("id", QJsonValue::fromVariant(i.id));
                    Player::Ptr player = m_datamodel->getPlayerByID(i.id);
                    player_obj.insert("player_name", QJsonValue::fromVariant(QString::fromStdString(player->getPlayerName())));

                    array.push_back(player_obj);
                }

                QJsonDocument res(array);
                int size = res.toJson().size();
                qDebug() << "sending: " << res;
                j.socket->write((char*)&size, 4);
                j.socket->write(res.toJson());
            }

            qDebug() << "state changed: ROUND";
            state = ROUND;
        }
    } else if (state == PRE_FIGHT) {
        ready_count++;
        if (clients.size() == ready_count) {
            ready_count = 0;
            qDebug() << "state changed: FIGHT";
            state = FIGHT;
            fight_init(m_battle_list[battle_count]);
        }
    } else if (state == END_FIGHT) {
        ready_count++;
        if (clients.size() == ready_count) {
            ready_count = 0;
            if (battle_count < m_battle_list.size()) {
                qDebug() << "state changed: PRE_FIGHT";
                state = PRE_FIGHT;
                send_battle(m_battle_list[battle_count], false);
            } else {
                send_state();
                qDebug() << "state changed: ROUND";
                state = ROUND;
            }
        }
    }
}

void TcpServer::handle_state(TcpClient& client, QJsonDocument& doc) {

    qDebug() << "state received";
    qDebug() << doc;
    if (state != ROUND) {
        return;
    }
    //TODO berechnungen / kämpfe
    QJsonArray array_state = doc.array();
    QJsonObject obj_state = array_state[1].toObject();
    m_datamodel->updateAll(obj_state);

    qDebug() << "ADSFSADFSADFS " << m_datamodel->createJson(m_datamodel->getPlayerByID(client.id));


    m_datamodel->printPlanets();
    m_datamodel->printPlayer();

    ready_count++;
    if (clients.size() == ready_count) {
        ready_count = 0;
        m_battle_list = m_datamodel->findBattles();
        m_datamodel->clearInvaders();
        qDebug() << "n battles: " << m_battle_list.size();
        //m_battle_list = std::vector<Battle::Ptr>{Battle::Ptr(nullptr)};

        battle_count = 0;
        // Relevant,
        //udpServer.start();

        if (m_battle_list.size() > 0) {
            state = PRE_FIGHT;
            qDebug() << "state changed: PRE_FIGHT";
            send_battle(m_battle_list[0], true);
        } else {
            state = ROUND;
            qDebug() << "state changed: ROUND";
            send_state();
        }
    }
}

void TcpServer::fight_init(Battle::Ptr battle)
{

    m_datamodel->printPlanets();

    AsteroidField asteroids(20);
    std::list<PhysicalObject::Ptr> asteroid_list;
    asteroids.getAsteroids(asteroid_list);

    qDebug() << "sending fight_init";

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

        int ship_count;
        if (battle->m_player1->getIdentity() == j.id) {
            ship_count = battle->m_numberShips1;
        } else {
            ship_count = battle->m_numberShips2;
        }

        udpServer->add_client(j.id, j.socket->peerAddress(), j.socket->peerPort(), position, ship_count * 10);
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
            int ship_count;

            qDebug() << "Battle adfsadf: " << battle->m_numberShips1 << " " <<  battle->m_numberShips2;
            if (battle->m_player1->getIdentity() == i.id) {
                ship_count = battle->m_numberShips1;
            } else {
                ship_count = battle->m_numberShips2;
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
            player_obj.insert("ship_count", QJsonValue::fromVariant(ship_count));
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

    udpServer->stop();
    //udpServer.reset();
    int ships_left = health_left / 10;
    if (health_left % 10 > 0) {
        ships_left++;
    }
    qDebug() << "end fight, updating data model";
    Battle::Ptr current_battle = m_battle_list[battle_count];

    if (id == current_battle->m_player1->getIdentity() == id) {
        current_battle->m_numberShipsLeft1 = ships_left;
    } else {
        current_battle->m_numberShipsLeft2 = ships_left;
    }

    current_battle->m_player1->RemovePlaneteFromList(current_battle->m_location);
    current_battle->m_player2->RemovePlaneteFromList(current_battle->m_location);

    qDebug() << "Planets fight end 1: winner id: " << id;
    m_datamodel->printPlanets();
    qDebug() << "Planets player 1";
    current_battle->m_player1->PrintPlanetsList();
    qDebug() << "Planets player 2";
    current_battle->m_player2->PrintPlanetsList();

    current_battle->m_location->setOwner(m_datamodel->getPlayerByID(id));
    m_datamodel->getPlayerByID(id)->addPlanet(current_battle->m_location);

    current_battle->m_location->setShips(ships_left);
    m_datamodel->getPlayerByID(id)->incShips(ships_left);
    current_battle->m_location->setInvaderShips(0);
    current_battle->m_location->setInvader(nullptr);

    qDebug() << "Planets fight end 2";
    m_datamodel->printPlanets();
    qDebug() << "Planets player 1";
    current_battle->m_player1->PrintPlanetsList();
    qDebug() << "Planets player 2";
    current_battle->m_player2->PrintPlanetsList();
    //udpServer.reset();


    battle_count++;
    if (battle_count > 0 && battle_count <= m_battle_list.size()) {
        Battle::Ptr prev_battle = m_battle_list[battle_count - 1];

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        send_battle(prev_battle, false);

        qDebug() << "state changed: END_FIGHT";
        state = END_FIGHT;
    }
}

void TcpServer::send_battle(Battle::Ptr battle, bool first) {
    for (auto client: clients) {
        qDebug() << "send_battle";
        QJsonArray array;
        array.push_back("battle");
        QJsonObject battle_json;
        battle_json.insert("planet_name", QJsonValue::fromVariant(QString::fromStdString(battle->m_location->getName())));
        battle_json.insert("player_name1", QJsonValue::fromVariant(QString::fromStdString(battle->m_player1->getPlayerName())));
        battle_json.insert("player_name2", QJsonValue::fromVariant(QString::fromStdString(battle->m_player2->getPlayerName())));
        battle_json.insert("ships1", QJsonValue::fromVariant(battle->m_numberShips1));
        battle_json.insert("ships2", QJsonValue::fromVariant(battle->m_numberShips2));

        if (!first) {
            battle_json.insert("ships_after1", QJsonValue::fromVariant(battle->m_numberShipsLeft1));
            battle_json.insert("ships_after2", QJsonValue::fromVariant(battle->m_numberShipsLeft2));
        }

        array.push_back(battle_json);
        QJsonDocument res(array);

        int size = res.toJson().size();
        client.socket->write((char*)&size, 4);
        client.socket->write(res.toJson());
    }
}

void TcpServer::handle_init(TcpClient& client, QJsonDocument& doc)
{
    qDebug() << "init received";
    qDebug() << doc;

    // Ziehe Daten aus Document
    QJsonArray array = doc.array();
    QJsonObject temp_name = array[1].toObject();
    //qDebug() << "array"<< array;
    //qDebug() << "temp_name" << temp_name;
    QString name(temp_name["player_name"].toString().toUtf8());
    //qDebug() << name;
    m_datamodel->constructPlayer(client.id, name.toStdString(), false);
    m_datamodel->getPlayerByID(client.id)->setPlayerName(name.toUtf8().constData());


    qDebug() << "Sending init_res";
    QJsonObject init_res;
    init_res.insert("id", QJsonValue::fromVariant(client.id));
    init_res.insert("map", QJsonValue::fromVariant("../models/"+QString::fromStdString(level)+".map"));

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
            } else if (ident == "state") {
                handle_state(i, doc);
            }
        }
    }
}

