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
    connect(q_server, SIGNAL(newConnection()), this, SLOT(onConnect()));

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

    QJsonArray test = json_doc.array();

    QString ident (test[0].toString().toUtf8());

    if (ident == "id_init")
    {
        char id = id_gen();
        QString name = test[1].toString().toUtf8();
        id_name.insert(std::pair<char, QString> (id, name));
        sender->write(id.toUTF8());
    }
    else if (ident == "strat_init")
    {
        sender->write(writeJSON(2).toJson());
    }
    else if (ident == "fight_init")
    {
        sender->write(writeJSON(3).toJson());
    }


}

char testserver_tcp::id_gen() {
    return 'x';
}

QJsonDocument testserver_tcp::writeJSON(int option)
{
    switch (option) {
        case 2:
            break;
        case 3:
            break;
        default: break;
    }
}

void testserver_tcp::onConnect()
{
    qDebug() << "connected..";
// need to grab the socket
    QTcpSocket *socket = q_server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}


int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    testserver_tcp server;

    return a.exec();
}
