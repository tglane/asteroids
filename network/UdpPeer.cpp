
#include "UdpPeer.hpp"
#include <QNetworkDatagram>

namespace asteroids
{
    UdpPeer::UdpPeer()
    {
        socket.bind(QHostAddress::Any);
        connect(&socket, &QUdpSocket::readyRead, this, &UdpPeer::handle_udp);
    }

    UdpPeer::UdpPeer(QHostAddress bind_addr, int port)
    {
        socket.bind(bind_addr, port);
        connect(&socket, &QUdpSocket::readyRead, this, &UdpPeer::handle_udp);
    }

    void UdpPeer::handle_udp()
    {
        while(socket.hasPendingDatagrams())
        {
            //std::cout << "============ handle_udp =============" << std::endl;
            QNetworkDatagram datagram = socket.receiveDatagram();
            QByteArray data = datagram.data();

            switch (data[0]) {
            case 'A':
                //std::cout << "received A" << std::endl;
                handle_ack(datagram);
                break;
            case 'P':
                //std::cout << "received P" << std::endl;
                handle_position_packet(datagram);
                break;
            case 'B':
                //std::cout << "received B" << std::endl;
                send_ack(datagram);
                handle_bullet_packet(datagram);
                break;
            case 'C':
                //std::cout << "received B" << std::endl;
                send_ack(datagram);
                handle_collision_packet(datagram);
                break;
            case 'M':
                send_ack(datagram);
                handle_missile_packet(datagram);
                break;
            default:
                std::cout << "received unknown packet: " << data[0] << std::endl;
            }
        }
    }


    void UdpPeer::handle_position_packet(QNetworkDatagram &datagram)
    {
        QByteArray data = datagram.data();
        if (data.length() < 9 + 9 * 4) {
            std::cout << "packet to short" << std::endl;
            return;
        }
        uint32_t id = *((uint32_t*)(data.data() + 5));

        asteroids::Vector3f position = bytes_to_vector(data.data() + 9);

        asteroids::Vector3f x_axis = bytes_to_vector(data.data() + 9 + 6 * 4);
        asteroids::Vector3f y_axis = bytes_to_vector(data.data() + 9 + 9 * 4);
        asteroids::Vector3f z_axis = bytes_to_vector(data.data() + 9 + 12 * 4);

        emit received_position(id, position, x_axis, y_axis, z_axis);
    }

    void UdpPeer::handle_bullet_packet(QNetworkDatagram &datagram)
    {
        QByteArray data = datagram.data();
        if (data.length() < 9 + 6 * 4) {
            std::cout << "packet to short" << std::endl;
            return;
        }
        uint32_t obj_id = *((int32_t*)(data.data() + 5));

        asteroids::Vector3f position = bytes_to_vector(data.data() + 9);
        asteroids::Vector3f velocity = bytes_to_vector(data.data() + 9 + 3 * 4);
        emit received_bullet(obj_id, position, velocity);
    }

void UdpPeer::handle_missile_packet(QNetworkDatagram& datagram)
{
    QByteArray data = datagram.data();
    if (data.length() < 9 + 12 * 4) {
        std::cout << "packet to short" << std::endl;
        return;
    }
    uint32_t obj_id = *((int32_t*)(data.data() + 5));

    asteroids::Vector3f position = bytes_to_vector(data.data() + 9);
    asteroids::Vector3f x = bytes_to_vector(data.data() + 9 + 3 * 4);
    asteroids::Vector3f y = bytes_to_vector(data.data() + 9 + 6 * 4);
    asteroids::Vector3f z = bytes_to_vector(data.data() + 9 + 9 * 4);
    emit received_missile(obj_id, position, x, y, z);
}

void UdpPeer::handle_ack(QNetworkDatagram &datagram)
    {
        std::cout << "handle_ack" << std::endl;
        QByteArray data = datagram.data();
        uint32_t id = *((uint32_t*)(data.data() + 5));
        uint32_t seq_nr = *((uint32_t*)(data.data() + 1));

        emit received_ack(id, seq_nr);
    }

    void UdpPeer::handle_collision_packet(QNetworkDatagram &datagram)
    {
        std::cout << "handle_collision" << std::endl;
        QByteArray data = datagram.data();
        uint32_t id = *((uint32_t*)(data.data() + 5));
        uint32_t seq_nr = *((uint32_t*)(data.data() + 1));

        uint32_t id_one = *((uint32_t*)(data.data() + 9));
        uint32_t id_two = *((uint32_t*)(data.data() + 13));

        emit received_collision(id, id_one, id_two);
    }

    Vector3f UdpPeer::bytes_to_vector(char *bytes)
    {
        asteroids::Vector3f vec;
        for (int i = 0; i < 3; i++) {
            vec[i] = *((float *)(bytes + i * 4));
        }
        return vec;
    }

    void UdpPeer::send_ack(QNetworkDatagram &datagram)
    {
        if (datagram.data().length() < 5) {
            std::cout << "packet to short" << std::endl;
            return;
        }
        char* data = datagram.data().data();
        uint32_t seq_nr = *((uint32_t*)(data + 1));

        std::cout << "sending ack " << seq_nr << std::endl;
        QByteArray reply_data;
        reply_data.append('A');
        reply_data.append((char *)(&seq_nr), 4);
        reply_data.append("\0\0\0\0", 4);

        socket.writeDatagram(datagram.makeReply(reply_data));
    }

    QByteArray UdpPeer::send_data(QHostAddress& addr, int port, QByteArray data)
    {
        socket.writeDatagram(data, addr, port);
        return data;
    }

    QByteArray UdpPeer::send_collision(QHostAddress& addr, int port, int seq_nr, int id, int obj_id1, int obj_id2)
    {
        std::cout << "sending col" << std::endl;
        QByteArray data;
        data.append('C');
        data.append((char *)(&seq_nr), 4);
        data.append((char *)(&id), 4);
        data.append((char *)(&obj_id1), 4);
        data.append((char *)(&obj_id2), 4);
        socket.writeDatagram(data, addr, port);
        return data;
    }

    QByteArray UdpPeer::send_position(QHostAddress& addr, int port, int seq_nr, Hittable& obj)
    {
        //std::cout << "sending  " << 'P' << " to " << client.id << " seq_nr " << seq_nr << std::endl;
        QByteArray data;

        int obj_id = obj.getId();
        data.append('P');
        data.append((char *)(&seq_nr), 4);
        data.append((char *)(&obj_id), 4);

        asteroids::Vector3f position = obj.getPosition();
        asteroids::Vector3f velocity(); //obj.getVelocity();
        asteroids::Vector3f x_axis = obj.getXAxis();


        asteroids::Vector3f y_axis = obj.getYAxis();
        asteroids::Vector3f z_axis = obj.getZAxis();


        data.append((char *)(&position[0]), 4);
        data.append((char *)(&position[1]), 4);
        data.append((char *)(&position[2]), 4);

        for (int i = 0; i < 12; i++) {
            data.append('\0');
        }

        data.append((char *)(&x_axis[0]), 4);
        data.append((char *)(&x_axis[1]), 4);
        data.append((char *)(&x_axis[2]), 4);

        data.append((char *)(&y_axis[0]), 4);
        data.append((char *)(&y_axis[1]), 4);
        data.append((char *)(&y_axis[2]), 4);

        data.append((char *)(&z_axis[0]), 4);
        data.append((char *)(&z_axis[1]), 4);
        data.append((char *)(&z_axis[2]), 4);


        // std::cout << " senging position: " << std::endl
        //           << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
        //           << " x: " << x_axis[0] << ", " <<  x_axis[1] << ", " <<  x_axis[2] << std::endl
        //           << " y: " << y_axis[0] << ", " <<  y_axis[1] << ", " <<  y_axis[2] << std::endl
        //           << " z: " << z_axis[0] << ", " <<  z_axis[1] << ", " <<  z_axis[2] << std::endl;

        socket.writeDatagram(data, addr, port);

        return data;
    }



    QByteArray UdpPeer::send_bullet(QHostAddress& addr, int port, int seq_nr, PhysicalBullet& bullet)
    {
        //std::cout << "sending  " << 'B' << " to " << client.id << " seq_nr " << seq_nr << std::endl;
        QByteArray data;

        int bullet_id = bullet.get_id();

        data.append('B');
        data.append((char *)(&seq_nr), 4);
        data.append((char *)(&bullet_id), 4);

        asteroids::Vector3f position = bullet.getPosition();
        asteroids::Vector3f velocity = bullet.direction();


        data.append((char *)(&position[0]), 4);
        data.append((char *)(&position[1]), 4);
        data.append((char *)(&position[2]), 4);

        data.append((char *)(&velocity[0]), 4);
        data.append((char *)(&velocity[1]), 4);
        data.append((char *)(&velocity[2]), 4);


        // std::cout << " senging position: " << std::endl
        //           << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
        //           << " x: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl;

        socket.writeDatagram(data, addr, port);

        return data;
    }

QByteArray UdpPeer::send_missile(QHostAddress& addr, int port, int seq_nr, PhysicalMissile& missile)
{
    //std::cout << "sending  " << 'B' << " to " << client.id << " seq_nr " << seq_nr << std::endl;
    QByteArray data;

    int missile_id = missile.getId();

    data.append('M');
    data.append((char *)(&seq_nr), 4);
    data.append((char *)(&missile_id), 4);

    asteroids::Vector3f position = missile.getPosition();
    asteroids::Vector3f x = missile.getXAxis();
    asteroids::Vector3f y = missile.getYAxis();
    asteroids::Vector3f z = missile.getZAxis();

    data.append((char *)(&position[0]), 4);
    data.append((char *)(&position[1]), 4);
    data.append((char *)(&position[2]), 4);

    data.append((char *)(&x[0]), 4);
    data.append((char *)(&x[1]), 4);
    data.append((char *)(&x[2]), 4);

    data.append((char *)(&y[0]), 4);
    data.append((char *)(&y[1]), 4);
    data.append((char *)(&y[2]), 4);

    data.append((char *)(&z[0]), 4);
    data.append((char *)(&z[1]), 4);
    data.append((char *)(&z[2]), 4);

    // std::cout << " senging position: " << std::endl
    //           << " p: " << position[0] << ", " <<  position[1] << ", " <<  position[2] << std::endl
    //           << " x: " << velocity[0] << ", " <<  velocity[1] << ", " <<  velocity[2] << std::endl;

    socket.writeDatagram(data, addr, port);

    return data;
}

}
