#include "view2D/StartingDialog.hpp"
#include "view2D/MainWindow2D.hpp"
#include <iostream>
#include <QNetworkInterface>

namespace strategy{

StartingDialog::StartingDialog(DataModel::Ptr model,tcpclient::Ptr tcp_client, TcpServer::Ptr tcp_server, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::StartingDialog())
{
    m_model = model;

    m_tcpclient = tcp_client;
    m_tcpserver = tcp_server;

    ui->setupUi(this);

    ui->Title->setStyleSheet("QLabel { color: white }");
    ui->NameLabel->setStyleSheet("QLabel { color: white }");
    ui->ServerAddressLabel->setStyleSheet("QLabel { color: white }");
    ui->ChooseMapLabel->setStyleSheet("QLabel { color: white}");
    ui->ChooseMapLabel->setVisible(false);
    ui->SelectMap->setVisible(false);

    ui->Name->setText("Siegbert");
    ui->ServerAddress->setText("127.0.0.1");

    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    QPushButton* startButton = ui->StartGame;
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startGame(bool)));

    QPushButton* exitButton = ui->ExitGame;
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));

    QCheckBox* hostSelect = ui->checkHost;
    connect(hostSelect, SIGNAL(stateChanged(int)), this, SLOT(selectMap(int)));

}

void StartingDialog::start_round() {
    m_model->switchWindow(DataModel::MAIN2D);
}

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
    if(!(ui->checkHost->isChecked()) && name != "" && name != "Please insert a name!")
    {
        std::cout <<"Versuchen wirs"<< std::endl;
        m_tcpclient = shared_ptr<tcpclient>(new tcpclient(m_model));
        connect(this, SIGNAL(connect_to_server(string, string)), m_tcpclient.get(), SLOT(connect_to_server(string, string)));
        connect(this, SIGNAL(endround_signal()), m_tcpclient.get(), SLOT(endround_slot()));
        emit connect_to_server(name, server_addr);

    }
    else if (ui->checkHost->isChecked())
    {
        m_tcpserver = shared_ptr<TcpServer>(new TcpServer(ui->SelectMap->currentText().toStdString()));
        ui->Name->setText("Server Starting..");
        ui->Name->setStyleSheet("QLineEdit { color: red }");

        QList<QHostAddress> list = QNetworkInterface::allAddresses();


        // very buggy, if more than 1 ipv4, it maybe shows the right result
        QString ipv4;
        for(int nIter=0; nIter<list.count(); nIter++)
        {
            if(!list[nIter].isLoopback())
                if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                    ipv4 = list[nIter].toString();
        }

        ui->ServerAddress->setText(ipv4);
        ui->ServerAddress->setStyleSheet("QLineEdit { color: yellow }");
    }
    else
    {
        ui->Name->setText("Please insert a name!");
        ui->Name->setStyleSheet("QLineEdit { color: red }");
    }
    
}

void StartingDialog::selectMap(int state)
{
    if(state == 2)
    {
        ui->SelectMap->setVisible(true);
        ui->ChooseMapLabel->setVisible(true);
        ui->SelectMap->addItem("Level-1");
        ui->SelectMap->addItem("Level-2");
        ui->ServerAddress->setEnabled(false);
        ui->Name->setEnabled(false);
    } else {
        ui->SelectMap->setVisible(false);
        ui->ChooseMapLabel->setVisible(false);
        ui->SelectMap->clear();
        ui->ServerAddress->setEnabled(true);
        ui->Name->setEnabled(true);
    }
}


}
