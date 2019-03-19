//
// Created by timog on 19.03.19.
//

#ifndef CPP18_ASTEROIDS_TESTCLIENT_TCP_HPP
#define CPP18_ASTEROIDS_TESTCLIENT_TCP_HPP

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

class testclient_tcp : public QObject {

    Q_OBJECT

public:

    explicit testclient_tcp(QObject* parent = 0);

    void doConnect();

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket* socket;

};


#endif //CPP18_ASTEROIDS_TESTCLIENT_TCP_HPP
