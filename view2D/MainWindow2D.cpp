/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"
#include "datamodel/Planet.hpp"
#include "datamodel/DataModel.hpp"
#include <QGraphicsOpacityEffect>
#include <QGraphicsView>
#include <iostream>
#include <QLine>
#include <QObject>

namespace strategy {

MainWindow2D::MainWindow2D(DataModel::Ptr model, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2D())
{
    m_model = model;
    m_model->addWindow(DataModel::MAIN2D, this);

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
    QPen outlinePenHighlight(Qt::white);
    outlinePenHighlight.setWidth(1);;

    std::map<int, Planet::Ptr> planets = m_model->getPlanets();

    // // Map für die Elipsen-Objekten im QGraphicsScene

    //Erstelle die Elipsen und füge sie in die Map und in die QGraphicsScene ein 
    for(int i = 0; i < (int)planets.size(); i++){
        Planet::Ptr p = planets.at(i);
        view_planets[i] = new MyEllipse(p->getPosX()/position_scale, p->getPosY()/position_scale);
        view_planets[i]->setZValue(1);
        scene->addItem(view_planets[i]);
        QVariant ellipse_ID(i);
        view_planets[i]->setData(1, ellipse_ID);
        connect(view_planets[i], SIGNAL(show_planetInfo(int)), this, SLOT(choose_planet(int)));
    }

    std::list<std::pair<int,int>> edges = m_model->getEdges();

    //Linien einzeichnen
    for(std::list<std::pair<int,int>>::iterator it=edges.begin(); it != edges.end(); ++it){
        std::pair<int,int> coordinates = *it;
        int pos_1 = coordinates.first;
        int pos_2 = coordinates.second;
        Planet::Ptr p1 = planets.at(pos_1);
        Planet::Ptr p2 = planets.at(pos_2);
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

    currentPlanet = -1;

}

void MainWindow2D::resizeEvent(QResizeEvent* event){
    ui->Map->fitInView(0, 500, 500, 1, Qt::KeepAspectRatio);
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

void MainWindow2D::choose_planet(int id)
{
    Planet::Ptr p = m_model->getPlanetFromId(id);

    MyEllipse* ellipse = getEllipseById(id);
    if(id == currentPlanet){
        QPixmap pix("../models/surface/neutral1.jpg");
        ellipse->myBrush = QBrush(pix);
        currentPlanet = -1;
        ellipse->myPen = QPen(Qt::black,1);
    }else{
        if(currentPlanet!=-1){
        MyEllipse* otherEllipse = getEllipseById(currentPlanet);
        QPixmap otherpix("../models/surface/neutral2.jpg");
        otherEllipse->myBrush = QBrush(otherpix);
        otherEllipse->myPen = QPen(Qt::black,1);
        otherEllipse->update();
        }
        QPixmap pix("../models/surface/neutral2.jpg");
        ellipse->myBrush = QBrush(pix);
        currentPlanet = id;
        ellipse->myPen = QPen(Qt::white,1);
    }

    // Planeteninfo ausfüllen
    ui->PlanetName->setText(QString::fromStdString(p->getName()));
    ui->Info->setText("???");
    ui->MineNumber->setText(QString::number(p->getMines()));
    ui->ShipNumber->setText(QString::number(p->getShips()));

    std::list<Planet::Ptr> neighbour_list = p->getNeighbours();

    // Lösche die Anzahl der Schiffe des zuletzt ausgewählten Planeten aus der QComboBox
    ui->SendShipNumber->clear();
    // Fülle die QComboBox mit der aktuellen Anzahl an Schiffen
    for(int i = 0; i < p->getShips(); i++)
    {
        ui->SendShipNumber->addItem(QString::number(i + 1));
    }    

    // Lösche die Nachbarplaneten des zuletzt ausgewählten Planeten aus der QComboBox
    ui->DestionationPlanet->clear();
    // Fülle die QComboBox mit den aktuellen Nachbarn
    int j = neighbour_list.size();
    for(int i = 1; i <= j; i++)
    {
        ui->DestionationPlanet->addItem(QString::fromStdString(neighbour_list.front()->getName()));
        neighbour_list.pop_front();
    }

}

void MainWindow2D::endOfRound(bool click)
{
    bool succes = m_model->endOfRound();

    // fuck this "unused" warnings! :D
    if(succes);

    // TODO wait for response of server, block the window until all players are ready
}

void MainWindow2D::colonize(bool click /*, Planet* p*/)
{
    // TODO start colonization of Planet p
    //m_model->colonize(p);
    std::cout << "Colonize!" << std::endl;
}

void MainWindow2D::buildShip(bool click /*, Planet* p*/)
{
    // Ship should be accessible a round later

    //m_model->buildShip(p);
    std::cout << "Build Ship!" << std::endl;
}

void MainWindow2D::exitGame(bool click)
{
    QCoreApplication::quit();
}

MyEllipse* MainWindow2D::getEllipseById(int id)
{
    MyEllipse* ellipse = view_planets.at(id);
    return ellipse;
}

}
