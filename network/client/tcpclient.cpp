//
// Created by timog on 22.03.19.
//

#include <QDebug>
#include "tcpclient.hpp"

tcpclient::tcpclient(QString server_ip, QObject *parent) : m_server_ip(std::move(server_ip))
{
    m_socket = std::make_shared<QTcpSocket>(QTcpSocket(this));

    connect(m_socket.get(), SIGNAL(connected()), this, SLOT(send_init()));
    connect(m_socket.get(), SIGNAL(), this, SLOT());
}

void tcpclient::connect_to_server(string player_name)
{
    m_socket->connectToHost(m_server_ip, 1234);
    if(!m_socket->waitForConnected(10000));
    {
        qDebug() << "Error: " << m_socket->errorString();
    }

}

void tcpclient::send_init()
{

}
