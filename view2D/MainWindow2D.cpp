/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"
#include "datamodel/Planet.hpp"
#include "datamodel/DataModel.hpp"
#include <QGraphicsOpacityEffect>
#include <iostream>

namespace strategy {

MainWindow2D::MainWindow2D(DataModel *model, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2D())
{
    model = model;
    int planet_size = 20;
    float position_scale = 1;
    // Setup user interface
    ui->setupUi(this);
    
    scene = new QGraphicsScene(this);
    ui->Map->setScene(scene);

    
    ui->ContextMenue->setStyleSheet("background-color:#331155; border-radius:10px; color:#FFFFFF;");
    ui->Fight->setStyleSheet("background-color:#331155; color:#FFFFFF; border-radius:10px;");

    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(ui->ContextMenue);
    effect->setOpacity(0.7);
    ui->ContextMenue->setGraphicsEffect(effect);
    effect = new QGraphicsOpacityEffect(ui->Fight);
    effect->setOpacity(0.7);
    ui->Fight->setGraphicsEffect(effect);

    QBrush greenBrush(Qt::green);
    QBrush grayBrush(Qt::gray);
    QBrush redBrush(Qt::red);
    QPen outlinePenHighlight(Qt::white);
    outlinePenHighlight.setWidth(2);

    QPen outlinePen(Qt::black);
    outlinePen.setWidth(0);
    //scene->setBackgroundBrush(Qt::black);

    std::map<int, Planet*> planets = model->getPlanets();

    std::list<std::pair<int,int>> edges = model->getEdges();

    //Linien einzeichnen
    for(std::list<std::pair<int,int>>::iterator it=edges.begin(); it != edges.end(); ++it){
        std::pair<int,int> coordinates = *it;
        int pos_1 = coordinates.first;
        int pos_2 = coordinates.second;
        Planet *p1 = planets.at(pos_1);
        Planet *p2 = planets.at(pos_2);
        scene->addLine(p1->getPosX()/position_scale+planet_size/2,p1->getPosY()/position_scale+planet_size/2, p2->getPosX()/position_scale+planet_size/2, p2->getPosY()/position_scale+planet_size/2, outlinePenHighlight);
    }

    //Abhängig von Planeten machen
    for(int i = 0; i < planets.size(); i++){
        Planet *p = planets.at(i);
        scene->addEllipse(p->getPosX()/position_scale, p->getPosY()/position_scale, planet_size, planet_size, outlinePen, greenBrush);
        
    }

    //Öffne das Fighter-Minigame testweise in neuem Fenster
    QPushButton* m_button = ui->Fight;
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(fight(bool)));
    
    //Löschen für nicht transparent
    ui->Map->setStyleSheet("background: transparent");
    QPixmap bkgnd("../models/box1.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    //Einfach kopiert

    // Add the matching event to the next-round-button
    QPushButton* m_nextRound = ui->NextRound;
    connect(m_nextRound, SIGNAL(clicked(bool)), this, SLOT(endOfRound(bool)));


    FighterWindow = NULL;
}

MainWindow2D::~MainWindow2D() 
{
    delete ui;
    if(FighterWindow != NULL)
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