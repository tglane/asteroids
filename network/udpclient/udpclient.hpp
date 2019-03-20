//
// Created by leon on 19.03.19.
//

#ifndef CPP18_ASTEROIDS_UDPCLIENT_HPP
#define CPP18_ASTEROIDS_UDPCLIENT_HPP

#include <QObject>
#include <QUdpSocket>

class udpclient: public QObject {
Q_OBJECT

public:
    explicit udpclient(QObject *parent = 0);
    void send_udp();

signals:

public slots:
    void readyRead();

private:
    QUdpSocket *socket;
    int seq_number;
};


#endif //CPP18_ASTEROIDS_UDPCLIENT_HPP
