#include "view2D/GameWindow.hpp"
#include "view2D/MainWindow2D.hpp"
#include "view2D/StartingDialog.hpp"
#include "view2D/SwitchingWindowInfo.hpp"
#include "view2D/EndWindow.hpp"
#include "view/MainWindow.hpp"

#include <QFileInfo>

namespace strategy{

GameWindow::GameWindow(DataModel::Ptr model, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::GameWindow())
{
    m_model = model;
    
    ui->setupUi(this);

    m_model->addMainWindow(this);

    m_tcpclient = std::make_shared<tcpclient>(m_model);

    MainWindow2D* strategywindow = new MainWindow2D(m_model);
    ui->centralwidget->addWidget(strategywindow);
    m_model->addWidget(DataModel::MAIN2D, strategywindow);
    //connect(strategywindow, SIGNAL(endround_signal()), m_tcpclient.get(), SLOT(endround_slot()));


    // Insert 3D Window into Stacked Widget
    /**MainWindow* fightwindow = new MainWindow("../models/level.xml");
    ui->centralwidget->addWidget(fightwindow);
    m_model->addWidget(DataModel::MAIN3D, fightwindow);
    tcp_client->set3DWindow(fightwindow);*/

    StartingDialog* startingDialog = new StartingDialog(m_model, m_tcpclient, m_tcpserver);
    //connect(startingDialog, SIGNAL(connect_to_server(string, string)), m_tcpclient.get(), SLOT(connect_to_server(string, string)));

    ui->centralwidget->addWidget(startingDialog);
    m_model->addWidget(DataModel::START, startingDialog);

    SwitchingWindowInfo* switchdialog = new SwitchingWindowInfo(m_model);
    ui->centralwidget->addWidget(switchdialog);
    m_model->addWidget(DataModel::SWITCH, switchdialog);
    m_tcpclient->set_switch_pointer(switchdialog);
    connect(switchdialog->getButton(), SIGNAL(clicked()), m_tcpclient.get(), SLOT(send_ready()));

    EndWindow* endwindow = new EndWindow(m_model);
    ui->centralwidget->addWidget(endwindow);
    m_model->addWidget(DataModel::END, endwindow);

    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // Create a Mediaplayer which plays some background music
    m_mediaplayer = new QMediaPlayer();
    m_mediaplayer->setMedia(QUrl::fromLocalFile(QFileInfo("../models/Interstellar-Soundtrack.mp3").absoluteFilePath()));
    //m_mediaplayer->play();

    connect(this, SIGNAL(play()), m_mediaplayer, SLOT(play()));
    connect(this, SIGNAL(stop()), m_mediaplayer, SLOT(stop()));
    connect(this, SIGNAL(pause()), m_mediaplayer, SLOT(pause()));

    std::cout << "Connected to slots" << std::endl;

    m_model->switchWindow(DataModel::START);

}

void GameWindow::start_round() {
    m_model->switchWindow(DataModel::MAIN2D);
}

QStackedWidget* GameWindow::content()
{
    return ui->centralwidget;
}


GameWindow::~GameWindow()
{
    if (ui)
        delete ui;
}


}
