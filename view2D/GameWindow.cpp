#include "view2D/GameWindow.hpp"
#include "view2D/MainWindow2D.hpp"
#include "view2D/StartingDialog.hpp"
#include "view2D/SwitchingWindowInfo.hpp"
#include "view2D/EndWindow.hpp"
#include "view/MainWindow.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace strategy{

GameWindow::GameWindow(DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::GameWindow())
{
    m_model = model;
    
    ui->setupUi(this);

    m_model->addMainWindow(this);

    MainWindow2D* strategywindow = new MainWindow2D(m_model);
    ui->centralwidget->addWidget(strategywindow);
    m_model->addWidget(DataModel::MAIN2D, strategywindow);


    // Insert 3D Window into Stacked Widget
    MainWindow* fightwindow = new MainWindow("../models/level.xml", m_model);
    ui->centralwidget->addWidget(fightwindow);
    m_model->addWidget(DataModel::MAIN3D, fightwindow);

    StartingDialog* startingDialog = new StartingDialog(m_model);
    ui->centralwidget->addWidget(startingDialog);
    m_model->addWidget(DataModel::START, startingDialog);

    SwitchingWindowInfo* switchdialog = new SwitchingWindowInfo(m_model);
    ui->centralwidget->addWidget(switchdialog);
    m_model->addWidget(DataModel::SWITCH, switchdialog);

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
    fs::path mediafile = "../models/Interstellar-Soundtrack.mp3";
    m_mediaplayer->setMedia(QUrl::fromLocalFile(QString::fromStdString(fs::absolute(mediafile).string())));

    //m_mediaplayer->play();

    connect(this, SIGNAL(play()), m_mediaplayer, SLOT(play()));
    connect(this, SIGNAL(stop()), m_mediaplayer, SLOT(stop()));
    connect(this, SIGNAL(pause()), m_mediaplayer, SLOT(pause()));

    std::cout << "Connected to slots" << std::endl;

    m_model->switchWindow(DataModel::START);

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