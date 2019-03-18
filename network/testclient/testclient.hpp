//
// Created by leon on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_TESTCLIENT_HPP
#define CPP18_ASTEROIDS_TESTCLIENT_HPP

#include <QObject>
#include <QUdpSocket>

class testclient: public QObject {
    Q_OBJECT

public:
    explicit testclient(QObject *parent = 0);
    void hello_udp();

signals:

public slots:
    void readyRead();

private:
    QUdpSocket *socket;
};


#endif //CPP18_ASTEROIDS_TESTCLIENT_HPP
