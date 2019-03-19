/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"
#include <iostream>



namespace strategy {

MainWindow2D::MainWindow2D(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2D())
{
    // Setup user interface
    ui->setupUi(this);
    
    scene = new QGraphicsScene(this);
    ui->Map->setScene(scene);

    QBrush greenBrush(Qt::green);
    QBrush grayBrush(Qt::gray);
    QBrush redBrush(Qt::red);
    QPen outlinePenHighlight(Qt::white);
    outlinePenHighlight.setWidth(2);

    QPen outlinePen(Qt::black);
    outlinePen.setWidth(0);
    scene->setBackgroundBrush(Qt::black);

    //Abhängig von Planeten machen
    for(int i=0;i<20;i++){
        scene->addEllipse(rand()%100, rand()%100, 20, 20, outlinePen, greenBrush);
    }

    //Öffne das Fighter-Minigame testweise in neuem Fenster
    QPushButton* m_button = ui->Fight;
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(fight(bool)));

}

MainWindow2D::~MainWindow2D() 
{
    delete ui;
    delete FighterWindow;
}

/*
 * @brief öffnet das Fighter-Minigame in neuem Fenster
 */
void MainWindow2D::fight(bool click)
{
    std::cout << "Fight" << std::endl;
    FighterWindow = new asteroids::MainWindow("../models/level.xml");
    FighterWindow->show();
}

}