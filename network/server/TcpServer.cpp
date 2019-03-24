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

    if(!server.listen(QHostAddress::Any, 1236))
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

    TcpClient client(last_id++, server.nextPendingConnection());
    clients.push_back(client);

    connect(client.socket.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void TcpServer::send_strat_init()
{
    for (auto client: clients)
    {
        QJsonArray array;
        array.push_back("strat_init");
        QJsonDocument doc(array);
        client.socket->write(doc.toJson());
    }
}

void TcpServer::send_state()
{
    for (auto client: clients)
    {
        QJsonArray array;
        array.push_back("fight_init");
        QJsonDocument doc(array);
        client.socket->write(doc.toJson());
    }
}
void TcpServer::handle_ready(TcpClient& client, QJsonDocument& doc)
{

    qDebug() << "ready received";
    qDebug() << doc;
    if (state == WAIT_READY) {
        //QJsonObject init;
        //init.insert("id", QJsonValue::fromVariant(client.id));

        QJsonArray array;
        array.push_back("strat_init");
        //array.push_back(init);
        QJsonDocument res(array);

        client.socket->write(res.toJson());

        ready_count++;
        if (clients.size() == ready_count) {
            send_strat_init();
            qDebug() << "state changed: FIGHT";
            state = FIGHT;
        }
    } else if (state == ROUND) {
        //TODO berechnungen / kämpfe
        send_state();
        state = FIGHT;
        udpServer.start();
        qDebug() << "state changed: FIGHT";
    }
}


void TcpServer::handle_init(TcpClient& client, QJsonDocument& doc)
{
    qDebug() << "init received";
    qDebug() << doc;

    QJsonObject init;
    init.insert("id", QJsonValue::fromVariant(client.id));

    QJsonArray array;
    array.push_back("init_resp");
    array.push_back(init);
    QJsonDocument res(array);

    client.socket->write(res.toJson());

    init_count++;
    qDebug() << init_count;
    if (init_count == 1) {
        qDebug() << "state changed: WAIT_READY";
        state = WAIT_READY;
    }
    qDebug() << client.socket->peerAddress();
    udpServer.add_client(client.id, client.socket->peerAddress());
}


void TcpServer::readyRead()
{
    for (auto i: clients)
    {
        if (i.socket->bytesAvailable() > 0)
        {
            QByteArray data = i.socket->readAll();
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
