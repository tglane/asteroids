//
// Created by leon on 19.03.19.
//

#include <iostream>
#include <math/Quaternion.hpp>

#include "udpclient.hpp"

udpclient::udpclient(QObject *parent) : QObject(parent)
{

    socket = new QUdpSocket(this);
    seq_number = 1;

    socket->bind(QHostAddress::LocalHost, 1234);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void udpclient::send_position(asteroids::Vector<float> position, asteroids::Vector<float> direction, asteroids::Quaternion rotation)
{
    QByteArray data;
    /* Append type of packet */
    data.append('P');

    /* Append sequence number of packet */
    char seqn_char[sizeof(int)];
    memcpy(seqn_char, &seq_number, sizeof(seqn_char));
    data.append(seqn_char, sizeof(seqn_char));
    seq_number++;

    /* Append id of player to packet */
    int id = 1;
    char id_char[sizeof(int)];
    memcpy(id_char, &id, sizeof(id_char));
    data.append(id_char);

    /* Append position to packet */
    char vec_char[sizeof(float)];
    memcpy(vec_char, &position[0], sizeof(vec_char));
    data.append(vec_char);
    memcpy(vec_char, &position[1], sizeof(vec_char));
    data.append(vec_char);
    memcpy(vec_char, &position[2], sizeof(vec_char));
    data.append(vec_char);

    /* Append velocity to packet */
    memcpy(vec_char, &direction[0], sizeof(vec_char));
    data.append(vec_char);
    memcpy(vec_char, &direction[1], sizeof(vec_char));
    data.append(vec_char);
    memcpy(vec_char, &direction[2], sizeof(vec_char));
    data.append(vec_char);

    /* Append rotation to packet */
    float tmp = rotation.getX();
    memcpy(vec_char, &tmp, sizeof(vec_char));
    data.append(vec_char);
    tmp = rotation.getY();
    memcpy(vec_char, &tmp, sizeof(vec_char));
    data.append(vec_char);
    tmp = rotation.getZ();
    memcpy(vec_char, &tmp, sizeof(vec_char));
    data.append(vec_char);
    tmp = rotation.getW();
    memcpy(vec_char, &tmp, sizeof(vec_char));
    data.append(vec_char);

    qDebug() << data;
    socket->writeDatagram(data, QHostAddress::LocalHost, 1235);
}



void udpclient::readyRead()
{
    int i = 0;
}