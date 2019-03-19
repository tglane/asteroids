//
// Created by leon on 18.03.19.
//

#include <QCoreApplication>
#include <iostream>
#include <stdlib.h>
#include "testclient.hpp"

testclient::testclient(QObject *parent) : QObject(parent) {

    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::LocalHost, 1234);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(hello_udp()));
    m_timer->start(1000/4);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void testclient::hello_udp() {
    QByteArray data;
    char target[sizeof(seq_number)];
    float x, y, z;
    switch(rand() % 3){
        case 0:
            data.append("P");
            seq_number++;
            memcpy(target, &seq_number, sizeof(seq_number));
            data.append(target, sizeof(target));
            x = rand() / (RAND_MAX + 1.);
            y = rand() / (RAND_MAX + 1.);
            z = rand() / (RAND_MAX + 1.);
            char coords[sizeof(float)];
            mempcpy(coords, &x , sizeof(float));
            data.append(coords, sizeof(coords));
            mempcpy(coords, &y , sizeof(float));
            data.append(coords, sizeof(coords));
            mempcpy(coords, &z , sizeof(float));
            data.append(coords, sizeof(coords));
            break;
        case 1:
            data.append("B");
            seq_number++;
            memcpy(target, &seq_number, sizeof(seq_number));
            data.append(target, sizeof(target));
            break;
        case 2:
            data.append("C");
            seq_number++;
            memcpy(target, &seq_number, sizeof(seq_number));
            data.append(target, sizeof(target));
            break;
    }

    socket->writeDatagram(data, QHostAddress::LocalHost, 1234);
}

void testclient::readyRead() {
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    char* data = buffer.data();
    char type;
    memcpy(&type, data, sizeof(char));
    data++;
    int newseqn;
    memcpy(&newseqn, data, sizeof(int));

    float x,y,z;
    data += sizeof(int);
    memcpy(&x, data, sizeof(float));
    data += sizeof(float);
    memcpy(&y, data, sizeof(float));
    data += sizeof(float);
    memcpy(&z, data, sizeof(float));

    std::cout << "Message: " << type << newseqn << " " << x << " " << y << " " << z << std::endl;

    QByteArray response;
    response.append("A");
    char seqnchar[sizeof(int)];
    memcpy(seqnchar, &newseqn, sizeof(int));
    response.append(seqnchar);
    char idchar[sizeof(int)];
    int id = 1;
    memcpy(idchar, &id, sizeof(int));
    response.append(idchar);

    socket->writeDatagram(response, QHostAddress::LocalHost, 1234);
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    testclient client;
    client.seq_number = 0;
    return a.exec();
}