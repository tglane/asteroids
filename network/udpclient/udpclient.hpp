//
// Created by leon on 19.03.19.
//

#ifndef CPP18_ASTEROIDS_UDPCLIENT_HPP
#define CPP18_ASTEROIDS_UDPCLIENT_HPP

#include <QObject>
#include <QUdpSocket>

#include "math/Vector.hpp"
#include "math/Quaternion.hpp"

class udpclient: public QObject {
Q_OBJECT

public:

    explicit udpclient(QObject *parent = 0);

    void send_position(asteroids::Vector<float> position, asteroids::Vector<float> direction, asteroids::Quaternion rotation);

    void send_bullet();

signals:

public slots:
    void readyRead();

private:
    QUdpSocket *socket;
    unsigned int seq_number;
};


#endif //CPP18_ASTEROIDS_UDPCLIENT_HPP
