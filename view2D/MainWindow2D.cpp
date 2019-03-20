/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"
#include "datamodel/Planet.hpp"
#include <iostream>

namespace strategy {

MainWindow2D::MainWindow2D(DataModel *model, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2D())
{
    model = model;
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

    std::map<int, Planet*> planets = model->getPlanets();

    //Abhängig von Planeten machen
    for(int i = 0; i < planets.size(); i++){
        Planet *p = planets.at(i);
        scene->addEllipse(p->getPosX()/25, p->getPosY()/25, 20, 20, outlinePen, greenBrush);
    }

    //Öffne das Fighter-Minigame testweise in neuem Fenster
    QPushButton* m_button = ui->Fight;
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(fight(bool)));

    // Add the matching event to the next-round-button
    QPushButton* m_nextRound = ui->NextRound;
    connect(m_nextRound, SIGNAL(clicked(bool)), this, SLOT(endOfRound(bool)));

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

void MainWindow2D::endOfRound(bool click)
{
    bool succes = model->endOfRound();

    // TODO wait for response of server, block the window until all players are ready
}

}