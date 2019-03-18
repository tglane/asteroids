//
// Created by leon on 18.03.19.
//

#include <QCoreApplication>
#include <iostream>
#include "testclient.hpp"

testclient::testclient(QObject *parent) : QObject(parent) {

    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::LocalHost, 1234);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void testclient::hello_udp() {
    QByteArray data;
    data.append("BUL");
    data.append(std::to_string(14).c_str());
    data.append(std::to_string(15).c_str());
    data.append(std::to_string(-899).c_str());
    socket->writeDatagram(data, QHostAddress::LocalHost, 1234);
    data = "BUL";
    data.append(std::to_string(89).c_str());
    data.append(std::to_string(29).c_str());
    data.append(std::to_string(56349).c_str());
    socket->writeDatagram(data, QHostAddress::LocalHost, 1234);
    data = "AST";
    data.append(std::to_string(14).c_str());
    data.append(std::to_string(14).c_str());
    data.append(std::to_string(-200).c_str());
    socket->writeDatagram(data, QHostAddress::LocalHost, 1234);
}

void testclient::readyRead() {
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << "Message: " << buffer;
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    testclient client;
    client.hello_udp();
    return a.exec();
}