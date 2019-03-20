//
// Created by leon on 18.03.19.
//

#ifndef CPP18_ASTEROIDS_TESTCLIENT_HPP
#define CPP18_ASTEROIDS_TESTCLIENT_HPP

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class testclient: public QObject {
    Q_OBJECT

public:
    explicit testclient(QObject *parent = 0);
    int seq_number;

signals:

public slots:
    void readyRead();
    void hello_udp();

private:
    QUdpSocket *socket;
    QTimer *m_timer;
};


#endif //CPP18_ASTEROIDS_TESTCLIENT_HPP
