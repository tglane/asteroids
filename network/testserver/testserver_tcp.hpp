//
// Created by timog on 19.03.19.
//

#ifndef CPP18_ASTEROIDS_TESTCLIENT_TCP_HPP
#define CPP18_ASTEROIDS_TESTCLIENT_TCP_HPP

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QDebug>
#include <map>

class testserver_tcp : public QObject {

    Q_OBJECT

public:

    explicit testserver_tcp(QObject* parent = 0);

    //void doConnect();

    char id_gen();

    QJsonDocument writeJSON(int option);

signals:

public slots:
    void onConnect();
    void onReadyRead();

private:
    QTcpServer* q_server;
    std::map<char, QString> id_name;

};


#endif //CPP18_ASTEROIDS_TESTCLIENT_TCP_HPP
