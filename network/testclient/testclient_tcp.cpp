//
// Created by timog on 19.03.19.
//

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#include "testclient_tcp.hpp"

testclient_tcp::testclient_tcp(QObject *parent) : QObject(parent) {}

void testclient_tcp::doConnect()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting";

    socket->connectToHost("131.173.191.196", 1234);
    if(!socket->waitForConnected(10000));
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void testclient_tcp::connected()
{
    qDebug() << "connected..";

    QString player_name("player1");

    /* Create Json Document with id_init package */
    QJsonObject init_object;
    init_object.insert("player_name", QJsonValue::fromVariant(player_name));

    QJsonArray init;
    init.push_back("id_init");
    init.push_back(init_object);

    QJsonDocument doc(init);
    qDebug() << doc.toJson();

    socket->write(doc.toJson());
}

void testclient_tcp::disconnected()
{
    qDebug() << "disconnected..";
}

void testclient_tcp::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void testclient_tcp::readyRead()
{
    qDebug() << "reading...";

    /* Read answer from server and parse it to json */
    QByteArray data(socket->readAll());
    QString s(data);
    QJsonDocument recv_json = QJsonDocument::fromJson(s.toUtf8());
    // parse id from recv_json


    // parse fight_init

    qDebug() << s;
}

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    testclient_tcp client;

    client.doConnect();

    return a.exec();
}
