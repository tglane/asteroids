#include "view2D/StartingDialog.hpp"
#include "view2D/MainWindow2D.hpp"
#include <iostream>
#include <string>

namespace strategy{

StartingDialog::StartingDialog(DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::StartingDialog())
{
    m_model = model;
    m_model->addWindow(DataModel::START, this);

    ui->setupUi(this);

    ui->Title->setStyleSheet("QLabel { color: white }");
    ui->NameLabel->setStyleSheet("QLabel { color: white }");
    ui->ServerAddressLabel->setStyleSheet("QLabel { color: white }");

    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    QPushButton* startButton = ui->StartGame;
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startGame(bool)));

    QPushButton* exitButton = ui->ExitGame;
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));
    
}

StartingDialog::~StartingDialog()
{
    
}

void StartingDialog::exitGame(bool clicked)
{
    QCoreApplication::quit();
}

void StartingDialog::startGame(bool click)
{
    std::string name = ui->Name->text().toStdString();
    if(name != "")
    {
        //m_model->getThisPlayer()->setPlayerName(name);
        // Call switching mechanism of datamodel
        m_model->switchWindow(DataModel::MAIN2D);
        this->setVisible(false);
    }
    else
    {
        ui->Name->setText("Please insert a name!");
        ui->Name->setStyleSheet("QLineEdit { color: red }");
    }
    
}


}