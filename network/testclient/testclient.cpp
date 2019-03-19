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
    switch(rand() % 3){
        case 0: data.append("P"); break;
        case 1: data.append("B"); break;
        case 2: data.append("C"); break;
    }

    //sprintf(target, "%04d", seq_number++ );
    //double r = rand() / (RAND_MAX + 1.);
    //sprintf(target, "%04d", seq_number++ );
    seq_number++;
    memcpy(target, &seq_number, sizeof(seq_number));
    data.append(target);
    socket->writeDatagram(data, QHostAddress::LocalHost, 1234);
}

void testclient::readyRead() {
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    char* data = buffer.data();
    data++;
    int newseqn;
    memcpy(&newseqn, data, sizeof(int));

    std::cout << "Message: " << newseqn << std::endl;

    /**QByteArray data;
    data.append("A");
    char target[5];
    sprintf(target, "%04d", seq_number++ );
    socket->writeDatagram(data, QHostAddress::LocalHost, 1234);*/
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    testclient client;
    client.seq_number = 0;
    return a.exec();
}