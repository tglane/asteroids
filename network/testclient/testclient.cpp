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
    int typ = rand() % 3;
    switch(typ) {
        case 0:
            //Typ
            data.append("P");
            break;
        case 1:
            //Typ
            data.append("B");
            break;
        case 2:
            //Typ
            data.append("C");
            break;
    }

    //Sequenznummer
    seq_number++;
    memcpy(target, &seq_number, sizeof(seq_number));
    data.append(target, sizeof(target));
    //ID
    int id = 1;
    char idchar[sizeof(int)];
    memcpy(idchar, &id, sizeof(int));
    data.append(idchar, sizeof(idchar));

    switch(typ) {
        case 0: case 1:
            //Pos
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
            std::cout << "Message send: " << typ << seq_number << " " << id << " " << x << " " << y << " " << z << std::endl;
            //Mov
            mempcpy(coords, &x , sizeof(float));
            data.append(coords, sizeof(coords));
            mempcpy(coords, &y , sizeof(float));
            data.append(coords, sizeof(coords));
            mempcpy(coords, &z , sizeof(float));
            data.append(coords, sizeof(coords));
            //Rot
            mempcpy(coords, &x , sizeof(float));
            data.append(coords, sizeof(coords));
            mempcpy(coords, &y , sizeof(float));
            data.append(coords, sizeof(coords));
            mempcpy(coords, &z , sizeof(float));
            data.append(coords, sizeof(coords));
            break;
        case 2:
            //ID1
            data.append(idchar, sizeof(idchar));
            //ID2
            id = 2;
            char idchar[sizeof(int)];
            memcpy(idchar, &id, sizeof(int));
            data.append(idchar, sizeof(idchar));
            std::cout << "Message send: " << typ << seq_number << " "<< 1 << " "  << 1 << " " << 2 << std::endl;
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

    //Typ
    char* data = buffer.data();
    char typ;
    memcpy(&typ, data, sizeof(char));

    //Seqnr
    data += sizeof(char);
    int newseqn;
    memcpy(&newseqn, data, sizeof(int));

    //ID
    int id;
    data += sizeof(int);
    memcpy(&id, data, sizeof(int));

    if(typ == 'P' || typ == 'B') {
        //Pos
        float x, y, z;
        data += sizeof(int);
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        std::cout << "Message recv: " << typ << newseqn << " " << id << " " << x << " " << y << " " << z << std::endl << std::endl;
    }else if(typ == 'C'){
        int id1;
        data += sizeof(int);
        memcpy(&id1, data, sizeof(float));
        int id2;
        data += sizeof(int);
        memcpy(&id2, data, sizeof(float));
        std::cout << "Message recv: " << typ << newseqn << " "<< id << " "  << id1 << " " << id2 << std::endl << std::endl;
    } else{
        std::cout << "Message recv: " << typ << newseqn << std::endl;
    }

    //Ack
    QByteArray response;
    response.append("A");
    //Seqnr
    char seqnchar[sizeof(int)];
    memcpy(seqnchar, &newseqn, sizeof(int));
    response.append(seqnchar);
    //Id
    char idchar[sizeof(int)];
    id = 1;
    memcpy(idchar, &id, sizeof(int));
    response.append(idchar);

    socket->writeDatagram(response, QHostAddress::LocalHost, 1235);
}

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    testclient client;
    client.seq_numbher = 0;
    srand (time(NULL));
    return a.exec();
}