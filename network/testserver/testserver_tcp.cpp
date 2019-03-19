//
// Created by timog on 19.03.19.
//

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#include "testserver_tcp.hpp"

testserver_tcp::testserver_tcp(QObject *parent) : QObject(parent) {
    q_server = new QTcpServer(this);
    connect(q_server, SIGNAL(newConnection()), this, SLOT(doStuff()));

    qDebug() << "connecting";

    if(!q_server->listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Error: " << q_server->errorString();
    } else
    {
        qDebug() << "server startet";
    }
}

void testserver_tcp::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();

    QString json_String (datas);

    QJsonDocument json_doc = QJsonDocument::fromJson(json_String.toUtf8());

    qDebug() << "bumbun: " << json_doc.array();;
}

QJsonDocument writeJSON()
{

}

void testserver_tcp::doStuff()
{
    qDebug() << "connected..";

    QString player_name("Server");

    /* Create Json Document with id_init package*/
    QJsonObject init_object;
    init_object.insert("player_name", QJsonValue::fromVariant(player_name));

    QJsonArray init;
    init.push_back("id_init");
    init.push_back(init_object);

    QJsonDocument doc(init);
// need to grab the socket
    QTcpSocket *socket = q_server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    socket->write(doc.toJson());
    socket->flush();
}


int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    testserver_tcp server;

    return a.exec();
}
