#include "view2D/GameWindow.hpp"
#include "view2D/MainWindow2D.hpp"
#include "view2D/StartingDialog.hpp"
#include "view2D/EndWindow.hpp"
#include "view/MainWindow.hpp"

namespace strategy{

GameWindow::GameWindow(DataModel::Ptr model, tcpclient::Ptr tcp_client, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::GameWindow())
{
    m_model = model;
    
    ui->setupUi(this);

    m_model->addMainWindow(ui->centralwidget);

    MainWindow2D* strategywindow = new MainWindow2D(m_model);
    ui->centralwidget->addWidget(strategywindow);
    m_model->addWidget(DataModel::MAIN2D, strategywindow);
    connect(strategywindow, SIGNAL(endround_signal()), tcp_client.get(), SLOT(endround_slot()));

    // Insert 3D Window into Stacked Widget
    /*MainWindow* fightwindow = new MainWindow("../models/level.xml");
    ui->centralwidget->addWidget(fightwindow);
    m_model->addWidget(DataModel::MAIN3D, fightwindow);*/

    StartingDialog* startingDialog = new StartingDialog(m_model);
    connect(startingDialog, SIGNAL(connect_to_server(string, string)), tcp_client.get(), SLOT(connect_to_server(string, string)));

    ui->centralwidget->addWidget(startingDialog);
    m_model->addWidget(DataModel::START, startingDialog);

    EndWindow* endwindow = new EndWindow(m_model);
    ui->centralwidget->addWidget(endwindow);
    m_model->addWidget(DataModel::END, endwindow);


    m_model->switchWindow(DataModel::START);

    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

}

void GameWindow::start_round() {
    m_model->switchWindow(DataModel::MAIN2D);
}

GameWindow::~GameWindow()
{

}


}