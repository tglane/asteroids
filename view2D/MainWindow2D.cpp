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

    // Map mit den Planeten-Objekten aus dem DataModel
    std::map<int, Planet*> planets = model->getPlanets();

    // Map für die Elipsen-Objekten im QGraphicsScene
    std::map<int, MyEllipse*> view_planets;

    //Erstelle die Elipsen und füge sie in die Map und in die QGraphicsScene ein 
    for(int i = 0; i < (int)planets.size(); i++){
        Planet *p = planets.at(i);
        view_planets[i] = new MyEllipse(p->getPosX()/position_scale, p->getPosY()/position_scale);
        scene->addItem(view_planets[i]);
        QVariant ellipse_ID(i);
        //view_planets[i]->setData(1, ellipse_ID);
    }

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

    // Start colonizing the selected Planet, how we get selected Planet?
    // Button should only be selectable if a neighbour of the selected Planet is owned
    QPushButton* m_colonize = ui->Colonize;
    m_colonize->setEnabled(false);
    connect(m_colonize, SIGNAL(clicked(bool)), this, SLOT(colonize(bool)));

    // Build a ship on selected Planet
    // Button should only be selectable if this Planet is owned
    QPushButton* m_buildShip = ui->BuildShip;
    connect(m_buildShip, SIGNAL(clicked(bool)), this, SLOT(buildShip(bool)));

    // at the beginning no planet is selected so this widget is not visible
    // ui->PlanetInfo->setVisible(false);

    QPushButton* m_exit = ui->ExitGame;
    connect(m_exit, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));


    // Somehow there's a Segmentation fault if the Fighterwindow is initialized here like
    // FighterWindow = new asteroids::MainWindow("...")
    FighterWindow = NULL;

}

MainWindow2D::~MainWindow2D() 
{
    if(ui)
        delete ui;
    if(FighterWindow != NULL)
        delete FighterWindow;
}


void MainWindow2D::fight(bool click)
{
    std::cout << "Fight" << std::endl;
    FighterWindow = new asteroids::MainWindow("../models/level.xml");
    FighterWindow->show();
}

void MainWindow2D::choose_planet()
{
    std::cout << "Planet angeklickt" << std::endl;
}

void MainWindow2D::endOfRound(bool click)
{
    bool succes = model->endOfRound();

    // fuck this "unused" warnings! :D
    if(succes);

    // TODO wait for response of server, block the window until all players are ready
}

void MainWindow2D::colonize(bool click /*, Planet* p*/)
{
    // TODO start colonization of Planet p
    //model->colonize(p);
    std::cout << "Colonize!" << std::endl;
}

void MainWindow2D::buildShip(bool click /*, Planet* p*/)
{
    // Ship should be accessible a round later

    //model->buildShip(p);
    std::cout << "Build Ship!" << std::endl;
}

void MainWindow2D::exitGame(bool click)
{
    QCoreApplication::quit();
}

}