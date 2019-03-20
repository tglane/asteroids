//
// Created by leon on 19.03.19.
//

#include <iostream>
#include "udpclient.hpp"

udpclient::udpclient(QObject *parent) : QObject(parent) {

    socket = new QUdpSocket(this);
    seq_number = 1;

    socket->bind(QHostAddress::LocalHost, 1234);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void udpclient::send_udp() {
    int i = 0;
}

void udpclient::readyRead() {
    int i = 0;
}