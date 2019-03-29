#include "view2D/StartingDialog.hpp"
#include "view2D/MainWindow2D.hpp"
#include <iostream>

#include <QNetworkInterface>
#include <QRandomGenerator>

namespace strategy{

StartingDialog::StartingDialog(DataModel::Ptr model,tcpclient::Ptr tcp_client, TcpServer::Ptr tcp_server, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::StartingDialog())
{
    // registrate data model, client and server
    m_model = model;
    m_tcpclient = tcp_client;
    m_tcpserver = tcp_server;

    // layout components
    ui->setupUi(this);

    // select random subtitle
    QRandomGenerator generator(QDateTime::currentMSecsSinceEpoch()/1000);
    double d;
    if((d = generator.generateDouble()) < 0.16)
    {
        ui->Subtitle->setText("---Resurrection---");
    }
    else if(d < 0.33)
    {
        ui->Subtitle->setText("---Awakening---");
    }
    else if(d < 0.5)
    {
        ui->Subtitle->setText("---Apocalypse---");
    }
    else if(d < 0.66)
    {
        ui->Subtitle->setText("---Revolution---");
    }
    else if(d < 0.83)
    {
        ui->Subtitle->setText("---Afterlife---");
    }
    else
    {
        ui->Subtitle->setText("---Junior---");
    }

    // set default text, IP-address, hide components
    ui->StartGame->setText("Verbinde");
    ui->ChooseMapLabel->setVisible(false);
    ui->SelectMap->setVisible(false);
    ui->Name->setText("Please insert your name!");
    ui->ServerAddress->setText("127.0.0.1");

    // set the windows background
    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // connect buttons and checkbox with the matching slots
    QPushButton* startButton = ui->StartGame;
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startGame(bool)));

    QPushButton* exitButton = ui->ExitGame;
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));

    QCheckBox* hostSelect = ui->checkHost;
    connect(hostSelect, SIGNAL(stateChanged(int)), this, SLOT(selectMap(int)));

}

/*void StartingDialog::start_round() {
    m_model->switchWindow(DataModel::MAIN2D);
}*/

StartingDialog::~StartingDialog()
{
    if (ui)
        delete ui;
}

void StartingDialog::exitGame(bool clicked)
{
    QCoreApplication::quit();
}

void StartingDialog::startGame(bool click)
{
    std::string name = ui->Name->text().toStdString();
    std::string server_addr = ui->ServerAddress->text().toStdString();
    if(!(ui->checkHost->isChecked()) && name != "" && name != "Please insert your name!")
    {
        connect(this, SIGNAL(connect_to_server(string, string)), m_tcpclient.get(), SLOT(connect_to_server(string, string)));
        connect(m_model->getWidget(DataModel::MAIN2D), SIGNAL(endround_signal()), m_tcpclient.get(), SLOT(endround_slot()));
        connect(this, SIGNAL(endround_signal()), m_tcpclient.get(), SLOT(endround_slot()));
        ui->StartGame->setText("Verbinde...");
        //ui->StartGame->setStyleSheet("QLabel { color: yellow }");
        emit connect_to_server(name, server_addr);

    }
    else if (ui->checkHost->isChecked()) {
        if (m_tcpserver == nullptr) {
            ui->StartGame->setText("Restart");
            m_tcpserver = shared_ptr<TcpServer>(new TcpServer(ui->SelectMap->currentText().toStdString()));
            ui->Name->setText("Server running..");
            ui->NameLabel->setText("Server-Status");
            ui->Name->setStyleSheet("QLineEdit { color: red }");

            QList<QHostAddress> list = QNetworkInterface::allAddresses();
            QString ipv4;
            for (int nIter = 0; nIter < list.count(); nIter++) {
                if (!list[nIter].isLoopback())
                    if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
                        ipv4 = list[nIter].toString();
            }

            ui->ServerAddress->setText(ipv4);
            ui->ServerAddress->setStyleSheet("QLineEdit { color: green }");
        } else {
            m_tcpserver->server.close();
            m_tcpserver = shared_ptr<TcpServer>(new TcpServer(ui->SelectMap->currentText().toStdString()));
        }
    }
    else
    {
        ui->Name->setText("Please insert your name!");
        ui->Name->setStyleSheet("QLineEdit { color: red }");
    }
    
}

void StartingDialog::stop_server() {
    ui->Name->setText("Client disconnected");
    m_tcpserver->server.close();
    m_tcpserver = shared_ptr<TcpServer>(new TcpServer(ui->SelectMap->currentText().toStdString()));
}

void StartingDialog::selectMap(int state)
{
    if(state == 2)
    {
        ui->StartGame->setText("Host");
        ui->SelectMap->setVisible(true);
        ui->ChooseMapLabel->setVisible(true);
        ui->SelectMap->addItem("Level-1");
        ui->SelectMap->addItem("Level-2");
        ui->ServerAddress->setEnabled(false);
        ui->Name->setEnabled(false);
    } else {
        ui->StartGame->setText("Verbinde");
        ui->SelectMap->setVisible(false);
        ui->ChooseMapLabel->setVisible(false);
        ui->SelectMap->clear();
        ui->ServerAddress->setEnabled(true);
        ui->Name->setEnabled(true);
    }
}


}
