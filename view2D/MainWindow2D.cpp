/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"
#include "datamodel/Planet.hpp"
#include "datamodel/DataModel.hpp"
#include "view/MainWindow.hpp"
#include <QDesktopWidget>
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

    // Setup user interface
    ui->setupUi(this);
    
    resize(QDesktopWidget().availableGeometry(this).size());

    scene = new QGraphicsScene(this);
    ui->Map->setScene(scene);

    //StyleStuff
    ui->ContextMenue->setStyleSheet("background-color:#331155; border-radius:10px; color:#FFFFFF");
    ui->Fight->setStyleSheet("background-color:#331155; color:#FFFFFF; border-radius:10px;");
    ui->ExitGame->setStyleSheet("background-color:#220044");
    ui->NextRound->setStyleSheet("background-color:#220044");
    ui->BuildMine->setStyleSheet("background-color:#220044");
    ui->BuildShip->setStyleSheet("background-color:#220044");
    ui->SendShip->setStyleSheet("background-color:#220044");
    ui->Colonize->setStyleSheet("background-color:#220044");
    ui->SendShipNumber->setStyleSheet("background-color:#220044");
    ui->DestionationPlanet->setStyleSheet("background-color:#220044");

    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(ui->ContextMenue);
    effect->setOpacity(0.7);
    ui->ContextMenue->setGraphicsEffect(effect);
    effect = new QGraphicsOpacityEffect(ui->Fight);
    effect->setOpacity(0.7);
    ui->Fight->setGraphicsEffect(effect);
    QPen outlinePenHighlight(Qt::gray);
    outlinePenHighlight.setWidth(1);

    std::map<int, Planet::Ptr> planets = m_model->getPlanets();

    resizeEvent(NULL);

    int planet_size = 20;

    //Erstelle die Elipsen und füge sie in die Map und in die QGraphicsScene ein 
    for(int i = 0; i < (int)planets.size(); i++){
        Planet::Ptr p = planets.at(i);
        view_planets[i] = new MyEllipse(p->getPosX(), p->getPosY());
        view_planets[i]->setZValue(1);
        scene->addItem(view_planets[i]);
        QVariant ellipse_ID(i);
        view_planets[i]->setData(1, ellipse_ID);
        connect(view_planets[i], SIGNAL(show_planetInfo(int)), this, SLOT(choose_planet(int)));

        QGraphicsTextItem * io = new QGraphicsTextItem;
        io->setPos(p->getPosX() + planet_size/2,p->getPosY() - planet_size/2);
        io->setPlainText(QString::fromStdString(p->getName()));
        io->setDefaultTextColor(Qt::white);
        io->setFont(QFont("Helvetica",5));
        io->setZValue(1);
        scene->addItem(io);
    }

    std::list<std::pair<int,int>> edges = m_model->getEdges();

    //Linien einzeichnen
    for(std::list<std::pair<int,int>>::iterator it=edges.begin(); it != edges.end(); ++it){
        std::pair<int,int> coordinates = *it;
        int pos_1 = coordinates.first;
        int pos_2 = coordinates.second;
        Planet::Ptr p1 = planets.at(pos_1);
        Planet::Ptr p2 = planets.at(pos_2);
        scene->addLine(p1->getPosX()+planet_size/2, 
                    p1->getPosY()+planet_size/2, 
                    p2->getPosX()+planet_size/2, 
                    p2->getPosY()+planet_size/2, 
                    outlinePenHighlight);
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
    //m_colonize->setEnabled(false);
    connect(m_colonize, SIGNAL(clicked(bool)), this, SLOT(colonize(bool)));

    // Build a ship on selected Planet
    // Button should only be selectable if this Planet is owned
    QPushButton* m_buildShip = ui->BuildShip;
    connect(m_buildShip, SIGNAL(clicked(bool)), this, SLOT(buildShip(bool)));

    // Build a mine on selected Planet
    // Button should only be selectable if this Planet is owned
    QPushButton* m_buildMine = ui->BuildMine;
    connect(m_buildMine, SIGNAL(clicked(bool)), this, SLOT(buildMine(bool)));

    QPushButton* m_sendShips = ui->SendShip;
    connect(m_sendShips, SIGNAL(clicked(bool)), this, SLOT(sendShips(bool)));

    // at the beginning no planet is selected so this widget is not visible
    // ui->PlanetInfo->setVisible(false);

    QPushButton* m_exit = ui->ExitGame;
    connect(m_exit, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));

    currentPlanet = -1;

    ui->PlanetInfo->setVisible(false);

    updatePlayerInfo();

    // Insert 3D Window into Stacked Widget
    MainWindow* fightwindow = new MainWindow("../models/level.xml", m_model);
    ui->centralwidget->addWidget(fightwindow);

    std::cout << ui->centralwidget->indexOf(fightwindow) << std::endl;

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
    ui->centralwidget->setCurrentIndex(1);
}

void MainWindow2D::choose_planet(int id)
{
    updatePlanetInfo(id);

    std::map<int, Planet::Ptr> planets = m_model->getPlanets();

    //Higlighting of the planets
    MyEllipse* ellipse = getEllipseById(id);
    //wieder entmarkieren 
    if(id == currentPlanet){
        ui->PlanetInfo->setVisible(false);
        if(planets.at(id)->getOwner()==m_model->getSelfPlayer()){
            QPixmap pix("../models/surface/my1.jpg");
            ellipse->myBrush = QBrush(pix);
        } 
        
        // TODO Players are now saved in a map with their id
        //      iterate over all players if getOwner() != NULL
        else if (planets.at(id)->getOwner()==m_model->getEnemyPlayer(1)){
            QPixmap pix("../models/surface/other1.jpg");
            ellipse->myBrush = QBrush(pix);
        } else{
            QPixmap pix("../models/surface/neutral1.jpg");
            ellipse->myBrush = QBrush(pix);  
        }
        currentPlanet = -1;
        ellipse->myPen = QPen(Qt::black,1);
    }else{
        ui->PlanetInfo->setVisible(true);
        //Vorherigen Planet enhighliten
        if(currentPlanet!=-1){
            MyEllipse* otherEllipse = getEllipseById(currentPlanet);
            if(planets.at(currentPlanet)->getOwner()==m_model->getSelfPlayer()){
                QPixmap otherpix("../models/surface/my1.jpg");
                ellipse->myBrush = QBrush(otherpix);
            // TODO Players are now saved in a map with their id
            //      iterate over all players if getOwner() != NULL
            } else if (planets.at(currentPlanet)->getOwner()==m_model->getEnemyPlayer(1)){
                QPixmap otherpix("../models/surface/other1.jpg");
                otherEllipse->myBrush = QBrush(otherpix);
            } else{
                QPixmap otherpix("../models/surface/neutral1.jpg");
                otherEllipse->myBrush = QBrush(otherpix);  
            }
            otherEllipse->myPen = QPen(Qt::black,1);
            otherEllipse->update();
        }
        //Anklicken und highlighten
        if(planets.at(id)->getOwner()==m_model->getSelfPlayer()){
            QPixmap pix("../models/surface/my2.jpg");
            ellipse->myBrush = QBrush(pix);
        // TODO Players are now saved in a map with their id
        //      iterate over all players if getOwner() != NULL
        } else if (planets.at(id)->getOwner()==m_model->getEnemyPlayer(1)){
            QPixmap pix("../models/surface/other2.jpg");
            ellipse->myBrush = QBrush(pix);
        } else{
            QPixmap pix("../models/surface/neutral2.jpg");
            ellipse->myBrush = QBrush(pix);  
        }
        currentPlanet = id;
        ellipse->myPen = QPen(Qt::white,1);
    }
}

void MainWindow2D::endOfRound(bool click)
{
    bool succes = m_model->endOfRound();

    // fuck this "unused" warnings! :D
    if(succes);


    updatePlayerInfo();
    updatePlanetInfo(currentPlanet);

    // TODO wait for response of server, block the window until all players are ready
}

void MainWindow2D::colonize(bool click)
{
    Planet::Ptr p = m_model->getPlanetFromId(currentPlanet);

    m_model->setStartPlanet(p);

    ui->Colonize->setVisible(false);
    std::cout << "Colonize!" << std::endl;
    std::cout<<currentPlanet<<std::endl;
    MyEllipse* otherEllipse = getEllipseById(currentPlanet);
    QPixmap otherpix("../models/surface/my2.jpg");
    otherEllipse->myBrush = QBrush(otherpix);
    otherEllipse->update();

    // Aktualisiere Informationen
    ui->Info->setText(QString::fromStdString(p->getOwner()->getPlayerName()));
    ui->ShipNumber->setText(QString::number(p->getShips()));
    updatePlanetInfo(currentPlanet);
    updatePlayerInfo();
}

void MainWindow2D::buildShip(bool click)
{
    // Ship should be accessible a round later
    Planet::Ptr p = m_model->getPlanetFromId(currentPlanet);

    // TODO: Fehlerbehandlung
    if (p->getOwner() == NULL)
    {
        std::cout << "Planet wird nicht besessen!" << std::endl;
        return;
    }

    if (m_model->buyShip(p, p->getOwner()))
    {
        std::cout << "Build Ship!" << std::endl;
        updatePlanetInfo(currentPlanet);
        updatePlayerInfo();
    } else {
        std::cout << "Fehler MainWindow2D: Build Ship!" << std::endl;
    }
}

void MainWindow2D::buildMine(bool click)
{
    Planet::Ptr p = m_model->getPlanetFromId(currentPlanet);

    // TODO: Fehlerbehandlung
    if (p->getOwner() == NULL)
    {
        std::cout << "Planet wird nicht besessen!" << std::endl;
        return;
    }
    if (p->getMinesBuild() > 0)
    {
        std::cout << "Planet besitzt bereits eine Mine" << std::endl;
        return;
    }

    if (m_model->buyMine(p, p->getOwner()))
    {
        updatePlanetInfo(currentPlanet);
        updatePlayerInfo();
        std::cout << "Build Mine!" << std::endl;
    } else {
        std::cout << "Fehler MainWindow2D: Build Mine!" << std::endl;
    }
    
}

void MainWindow2D::sendShips(bool click)
{
    std::cout << "Send Ship from " << m_model->getPlanetFromId(currentPlanet)->getName() << 
        " to " << ui->DestionationPlanet->currentText().toStdString() << std::endl;
    
    Planet::Ptr to = m_model->getPlanetFromName(ui->DestionationPlanet->currentText().toStdString());
    Planet::Ptr from = m_model->getPlanetFromId(currentPlanet);
    m_model->moveShips(from, to, ui->SendShipNumber->currentText().toInt());
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

void MainWindow2D::updatePlayerInfo()
{
    ui->SpielerInfoTable->setCellWidget(0, 1, 
        new QLabel(QString::fromStdString(m_model->getSelfPlayer()->getPlayerName())));
    ui->SpielerInfoTable->setCellWidget(1, 1, 
        new QLabel(QString::number(m_model->getSelfPlayer()->getRubin())));
    ui->SpielerInfoTable->setCellWidget(2, 1, 
        new QLabel(QString::number(m_model->getSelfPlayer()->getPlanets().size())));
    ui->SpielerInfoTable->setCellWidget(3, 1, 
        new QLabel(QString::number(m_model->getSelfPlayer()->getMines())));
    ui->SpielerInfoTable->setCellWidget(4, 1, 
        new QLabel(QString::number(m_model->getSelfPlayer()->getShips())));
}

void MainWindow2D::updatePlanetInfo(int id)
{
    Planet::Ptr p = m_model->getPlanetFromId(id);

    if(p->getOwner() != m_model->getSelfPlayer())
    {
        // Disable entsprechende Felder, wenn Planet nicht besessen wird
        ui->SendShip->setVisible(false);
        ui->BuildMine->setVisible(false);
        ui->BuildShip->setVisible(false);
        ui->SendShipNumber->setVisible(false);
        ui->DestionationPlanet->setVisible(false);
    } else {
        // Enable entsprechende Felder, wenn Planet besessen wird
        ui->SendShip->setVisible(true);
        ui->BuildMine->setVisible(true);
        ui->BuildShip->setVisible(true);
        ui->SendShipNumber->setVisible(true);
        ui->DestionationPlanet->setVisible(true);

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

    // Planeteninfo ausfüllen
    ui->PlanetName->setText(QString::fromStdString(p->getName()));
    ui->MineNumber->setText(QString::number(p->getMinesBuild()));
    ui->ShipNumber->setText(QString::number(p->getShips()));
    if (p->getOwner() == NULL)
    {
        ui->Info->setText("Niemand besitzt diesen Planeten!");
    }
    else
    {
        ui->Info->setText(QString::fromStdString(p->getOwner()->getPlayerName()));
    }
}

void MainWindow2D::showPlayerName()
{
    ui->SpielerInfoTable->setCellWidget(0, 1, 
        new QLabel(QString::fromStdString(m_model->getSelfPlayer()->getPlayerName())));
}

}
