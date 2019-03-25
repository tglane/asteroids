#include "TcpServer.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QApplication>

TcpServer::TcpServer()
    : server(this)
{
    state = WAITING;
    connect(&server, SIGNAL(newConnection()), this, SLOT(onConnect()));

    m_datamodel = DataModel::Ptr(new DataModel("../models/Level-1.map"));

    if(!server.listen(QHostAddress::Any, 1235))
    {
        qDebug() << "Error: " << server.errorString();
    }
    else
    {
        qDebug() << "Server started ";

    }
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
            m_battle_list = m_datamodel->findBattles();

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
    //m_datamodel->updateAll(obj_state);
}

void TcpServer::fight_init()
{
    udpServer = std::shared_ptr<UdpServer>(new UdpServer());
    connect(udpServer.get(),SIGNAL(fightEnd(int)), this, SLOT(fightEnd(int)));
    for (auto j: clients)
    {
        udpServer->add_client(j.id, j.socket->peerAddress(), j.socket->peerPort(), 10);
    }

    udpServer->start();

    QJsonArray array;
    array.push_back("fight_init");
}

void TcpServer::fightEnd(int id) {
    // ToDo Datamodel
    //udpServer.stop();
    udpServer.reset();
    send_battle();
}

void TcpServer::send_battle() {
    if (battle_count < m_battle_list.size()) {
        // ToDo
        fight_init();
        battle_count++;

    } else {
        state = ROUND;
        qDebug() << "state changed: ROUND";
        send_state();
    }
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
    init_res.insert("map", QJsonValue::fromVariant("../models/Level-1.map"));

    QJsonArray array_response;
    array_response.push_back("init_res");
    array_response.push_back(init_res);
    QJsonDocument res(array_response);

    int size = res.toJson().size();
    client.socket->write((char*)&size, 4);
    client.socket->write(res.toJson());

    init_count++;
    qDebug() << init_count;
    if (init_count == 1) {
        qDebug() << "state changed: WAIT_READY";
        state = WAIT_READY;
    }
    qDebug() << client.socket->peerAddress();

    udpServer->add_client(client.id, client.socket->peerAddress(), client.socket->peerPort(), 100);
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
