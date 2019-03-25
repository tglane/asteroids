#include "view2D/GameWindow.hpp"
#include "view2D/MainWindow2D.hpp"
#include "view2D/StartingDialog.hpp"
#include "view/MainWindow.hpp"

namespace strategy{

GameWindow::GameWindow(DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::GameWindow())
{
    m_model = model;
    
    ui->setupUi(this);

    m_model->addMainWindow(ui->centralwidget);

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

    m_model->switchWindow(DataModel::START);

    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

}

GameWindow::~GameWindow()
{

}


}