
#include "udpclient.hpp"

udpclient::udpclient(QObject *parent)
 : QObject(parent)
{

    socket = new QUdpSocket(this);
    seq_number = 1;

    socket->bind(QHostAddress::LocalHost, 1234);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void udpclient::send_position(asteroids::Vector<float> position, asteroids::Vector<float> xAxis, asteroids::Vector<float> yAxis, asteroids::Vector<float> zAxis)
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
    int id = 42 << 24;
    char id_char[sizeof(int)];
    memcpy(id_char, &id, sizeof(id_char));
    data.append(id_char, sizeof(id_char));

    /* Append position to packet */
    char vec_char[sizeof(float)];
    memcpy(vec_char, &position[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &position[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &position[2], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));

    /* Append rotation to packet */
    memcpy(vec_char, &xAxis[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &xAxis[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &xAxis[2], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));

    memcpy(vec_char, &yAxis[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &yAxis[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &yAxis[2], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));

    memcpy(vec_char, &zAxis[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &zAxis[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &zAxis[2], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));

    /* Send own position data to server */
    socket->writeDatagram(data, QHostAddress::LocalHost, 1235);
}

void udpclient::readyRead()
{
    /* Receive data */
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 sender_port;
    socket->readDatagram(buffer.data(), buffer.size(), &sender, &sender_port);

    /* Get type of received package */
    char* data = buffer.data();
    char type;
    memcpy(&type, data, sizeof(char));

    /* Parse sequnce number of package */
    data += sizeof(char);
    int recv_seq_nr;
    memcpy(&recv_seq_nr, data, sizeof(int));

    /* Parse ID from package */
    data += sizeof(int);
    int recv_id;
    memcpy(&recv_id, data, sizeof(int));
    data += sizeof(int);

    /* Handle different package types */
    switch(type)
    {
        case 'P':
            setPosFromPackage(recv_id, data);
            break;
        case 'B':
            break;
        case 'C':
            break;
        default:
            break;
    }
}

void udpclient::setPosFromPackage(int recv_id, char *data)
{
    int id = recv_id >> 24;
    if(id != 42) {
        float x, y, z;

        /* Set position of enemy fighter */
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        m_otherFighter->setPosition(asteroids::Vector<float>(x, y, z));

        /* Set rotation of enemy fighter */
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        m_otherFighter->setXAxis(asteroids::Vector<float>(x, y, z));

        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        m_otherFighter->setYAxis(asteroids::Vector<float>(x, y, z));

        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        m_otherFighter->setZAxis(asteroids::Vector<float>(x, y, z));

    }
}
