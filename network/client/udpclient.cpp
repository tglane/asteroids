
#include "udpclient.hpp"
#include <iostream>

udpclient::udpclient(QObject *parent)
 : QObject(parent)
{
    // enter id -- only for testing
    std::cout << "Enter player_id (int):" << std::endl;
    std::cin >> m_id;

    m_ip = "192.168.0.42";

    socket = std::make_shared<QUdpSocket>(this);
    seq_number = 1;

    socket->bind(QHostAddress(QString("0.0.0.0")), 1234);

    connect(socket.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void udpclient::send_position(asteroids::Vector<float> position, asteroids::Vector<float> velocity, asteroids::Vector<float> xAxis, asteroids::Vector<float> yAxis, asteroids::Vector<float> zAxis)
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
    int id = m_id << 24;
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

    /* Append velocity to package */
    memcpy(vec_char, &velocity[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &velocity[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &velocity[2], sizeof(vec_char));
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
    socket->writeDatagram(data, QHostAddress(m_ip), 1235);
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
            createNewBulletFromPackage(recv_seq_nr, recv_id, data);
            break;
        case 'C':
            recv_collision(recv_seq_nr, data);
            break;
        case 'A':
            recv_ack(recv_seq_nr, recv_id);
            break;
        default:
            break;
    }
}

void udpclient::init_fight_slot(QJsonObject init_data)
{
    for(const QString& key : init_data.keys())
    {
        QJsonValue value = init_data.value(key);
    }
}

void udpclient::setPosFromPackage(int recv_id, char *data)
{
    int id = recv_id >> 24;
    if(id != m_id) {
        float x, y, z;

        /* Set position of enemy fighter */
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        m_otherFighter->setPosition(asteroids::Vector<float>(x, y, z));

        /* Set direction/velocity of enemy fighter */
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        //m_otherFighter->setDirection(asteroids::Vector<float>(x, y, z));

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

        frame_count = 0;

    }
}

void udpclient::send_bullet(asteroids::Vector3f position, asteroids::Vector3f xAxis, asteroids::Vector3f zAxis)
{
    QByteArray data;
    /* Append type of packet */
    data.append('B');

    /* Append sequence number of packet */
    char seqn_char[sizeof(int)];
    memcpy(seqn_char, &seq_number, sizeof(seqn_char));
    data.append(seqn_char, sizeof(seqn_char));
    seq_number++;

    /* Append id of player to packet */
    int id = m_id << 24;
    id += m_physicsEngine->get_curr_bull_id();
    char id_char[sizeof(int)];
    memcpy(id_char, &id, sizeof(id_char));
    data.append(id_char, sizeof(id_char));

    asteroids::Bullet::Ptr bullet = make_shared<asteroids::Bullet>(asteroids::Bullet(position - zAxis * 42,
                                                                                     xAxis, 0, id));
    m_physicsEngine->addBullet(bullet);

    /* Append position to packet */
    char vec_char[sizeof(float)];
    memcpy(vec_char, &position[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &position[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &position[2], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));

    /* Append velocity to package */
    memcpy(vec_char, &xAxis[0], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &xAxis[1], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));
    memcpy(vec_char, &xAxis[2], sizeof(vec_char));
    data.append(vec_char, sizeof(vec_char));

    /* Send own position data to server */
    socket->writeDatagram(data, QHostAddress(m_ip), 1235);

    /* Add new bullet to not acknowledged */
    m_not_acknowledged.insert(std::pair<int, QByteArray>(seq_number, data));
}

void udpclient::recv_collision(int recv_seq_nr, char* data)
{
    std::cout << "collision received" << std::endl;

    /* Parse recevived IDs of colliding objects and process collisions */

    int id_one, id_two;
    memcpy(&id_one, data, sizeof(int));
    data += sizeof(int);
    memcpy(&id_two, data, sizeof(int));
    data += sizeof(int);
    //TODO parse health from package
    m_physicsEngine->process_collisions(id_one, id_two);

    /* Send acknowledge to server */
    QByteArray ack;
    ack.append('A');

    char seqn_char[sizeof(int)];
    memcpy(seqn_char, &recv_seq_nr, sizeof(seqn_char));
    ack.append(seqn_char, sizeof(seqn_char));

    int id = m_id << 24;
    char id_char[sizeof(int)];
    memcpy(id_char, &id, sizeof(id_char));
    ack.append(id_char, sizeof(id_char));

    socket->writeDatagram(ack, QHostAddress(m_ip), 1235);
}

void udpclient::createNewBulletFromPackage(int recv_seq_nr, int recv_id, char* data) {
    int id = recv_id >> 24;
    if (id != m_id) {
        float x, y, z;

        /* Set position of enemy fighter */
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        asteroids::Vector3f bull_pos(x, y, z);

        /* Set direction/velocity of enemy fighter */
        memcpy(&x, data, sizeof(float));
        data += sizeof(float);
        memcpy(&y, data, sizeof(float));
        data += sizeof(float);
        memcpy(&z, data, sizeof(float));
        data += sizeof(float);
        asteroids::Vector3f bull_vel(x, y, z);

        asteroids::Bullet::Ptr bull = std::make_shared<asteroids::Bullet>(asteroids::Bullet(bull_pos, bull_vel, m_id, recv_id));
        m_physicsEngine->addBullet(bull);

        /* Send acknowledge to server */
        QByteArray ack;
        ack.append('A');

        char seqn_char[sizeof(int)];
        memcpy(seqn_char, &recv_seq_nr, sizeof(seqn_char));
        ack.append(seqn_char, sizeof(seqn_char));

        int id = m_id << 24;
        char id_char[sizeof(int)];
        memcpy(id_char, &id, sizeof(id_char));
        ack.append(id_char, sizeof(id_char));

        socket->writeDatagram(ack, QHostAddress(m_ip), 1235);
    }
}

void udpclient::recv_ack(int recv_seq_nr, int recv_id)
{
    recv_id = recv_id >> 24;
    if(recv_id == m_id)
    {
        m_not_acknowledged.erase(recv_seq_nr);
    }
}

void udpclient::send_not_acknowledged()
{
    for(auto it = m_not_acknowledged.begin(); it != m_not_acknowledged.end(); it++)
    {
        if((seq_number - it->first) > 50)
        {
            m_not_acknowledged.erase(it->first);
        }

        //socket->writeDatagram(it->second, QHostAddress(m_ip), 1235);
    }
}
