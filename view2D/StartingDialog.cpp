#include "view2D/StartingDialog.hpp"
#include "view2D/MainWindow2D.hpp"
#include <iostream>

namespace strategy{

StartingDialog::StartingDialog(DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::StartingDialog())
{
    m_model = model;

    ui->setupUi(this);

    ui->Title->setStyleSheet("QLabel { color: white }");
    ui->NameLabel->setStyleSheet("QLabel { color: white }");
    ui->ServerAddressLabel->setStyleSheet("QLabel { color: white }");
    ui->ChooseMapLabel->setStyleSheet("QLabel { color: white}");
    ui->ChooseMapLabel->setVisible(false);
    ui->SelectMap->setVisible(false);

    ui->Name->setText("Siegbert");
    ui->ServerAddress->setText("192.168.0.60");

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
    if(name != "" && name != "Please insert a name!")
    {
        //m_model->getSelfPlayer()->setPlayerName(name);
        


        //ui->ServerAddress->setText("127.0.0.1");

        emit connect_to_server(name, ui->ServerAddress->text().toStdString());
        
        // Call switching mechanism of datamodel
        //m_model->switchWindow(DataModel::MAIN2D);
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
        ui->SelectMap->addItem("Level 1");
        ui->SelectMap->addItem("Level 2");
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
