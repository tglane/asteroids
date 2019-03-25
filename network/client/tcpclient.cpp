//
// Created by timog on 22.03.19.
//

#include <QDebug>
#include "tcpclient.hpp"

tcpclient::tcpclient(QString player_name, QString server_ip, QObject* parent)
    : m_player_name(std::move(player_name)), m_server_ip(std::move(server_ip))
{
    m_socket = std::make_shared<QTcpSocket>(this);

    connect(m_socket.get(), SIGNAL(connected()), this, SLOT(send_init()));
    connect(m_socket.get(), SIGNAL(readyRead()), this, SLOT(recv_json()));
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
    QJsonObject init_object;
    init_object.insert("player_name", QJsonValue::fromVariant(m_player_name));

    QJsonArray init_array;
    init_array.push_back("init");
    init_array.push_back(init_object);

    QJsonDocument doc(init_array);

    int size = doc.toJson().size();
    m_socket->write((char*) &size, sizeof(size));

    m_socket->write(doc.toJson());
    m_socket->flush();

    m_state = client_state::CONNECTING;
}

void tcpclient::recv_json()
{
    QByteArray recv_data(m_socket->readAll());
    QString recv_json(recv_data);
    QJsonDocument json_doc = QJsonDocument::fromJson(recv_json.toUtf8());
    QJsonArray recv_array = json_doc.array();

    if(recv_array[0] == "init_res")
    {
        process_init_res(recv_array[1].toObject());
    } else if(recv_array[0] == "strat_init")
    {
        process_strat_init(recv_array[1].toObject());
    } else if(recv_array[0] == "fight_init")
    {
        process_state(recv_array[1].toObject());
    } else
    {
        std::cout << m_state << " | " << recv_array[1].toString().toUtf8().constData() << std::endl;
    }
}

void tcpclient::process_init_res(QJsonObject recv_obj)
{


    QJsonArray init_array;
    init_array.push_back("ready");

    QJsonDocument doc(init_array);

    m_socket->write(doc.toJson());
    m_socket->flush();
}



void tcpclient::process_strat_init(QJsonObject recv_obj)
{

}

void tcpclient::process_state(QJsonObject recv_obj)
{

}
