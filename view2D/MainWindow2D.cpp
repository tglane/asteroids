/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"
#include "datamodel/Planet.hpp"
#include "datamodel/DataModel.hpp"
#include "view/MainWindow.hpp"
#include "view2D/StartingDialog.hpp"
#include <QDesktopWidget>
#include <QGraphicsOpacityEffect>
#include <QGraphicsView>
#include <iostream>
#include <QLine>
#include <QObject>
#include <QDialog>

namespace strategy {

MainWindow2D::MainWindow2D(DataModel::Ptr model, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2D())
{
    m_model = model;

    // Setup user interface
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->Map->setScene(scene);

    //StyleStuff
    ui->ContextMenue->setStyleSheet("background-color:#331155; border-radius:10px; color:#FFFFFF");
    ui->Fight->setStyleSheet("background-color:#442266; color:#FFFFFF; border-radius:10px;");
    ui->ExitGame->setStyleSheet("background-color:#110033");
    ui->NextRound->setStyleSheet("background-color:#110033");
    ui->BuildMine->setStyleSheet("background-color:#110033");
    ui->BuildShip->setStyleSheet("background-color:#110033");
    ui->SendShip->setStyleSheet("background-color:#110033");
    ui->Colonize->setStyleSheet("background-color:#110033");
    ui->SendShipNumber->setStyleSheet("background-color:#110033");
    ui->DestionationPlanet->setStyleSheet("background-color:#110033");
    ui->SpielerInfoTable->setStyleSheet("border-width:0px;");


    //Set sidebar look
    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(ui->ContextMenue);
    effect->setOpacity(0.7);
    ui->ContextMenue->setGraphicsEffect(effect);
    effect = new QGraphicsOpacityEffect(ui->Fight);
    effect->setOpacity(0.7);
    ui->Fight->setGraphicsEffect(effect);
    
    //initPlanets();

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


    // Add the matching event to the next-round-button
    QPushButton* m_nextRound = ui->NextRound;
    connect(m_nextRound, SIGNAL(clicked(bool)), this, SLOT(endOfRound(bool)));

    // Start colonizing the selected Planet, how we get selected Planet?
    // Button should only be selectable if a neighbour of the selected Planet is owned
    QPushButton* m_colonize = ui->Colonize;
    connect(m_colonize, SIGNAL(clicked(bool)), this, SLOT(colonize(bool)));

    // Build a ship on selected Planet
    // Button should only be selectable if this Planet is owned
    QPushButton* m_buildShip = ui->BuildShip;
    connect(m_buildShip, SIGNAL(clicked(bool)), this, SLOT(buildShip(bool)));

    // Build a mine on selected Planet
    // Button should only be selectable if this Planet is owned
    QPushButton* m_buildMine = ui->BuildMine;
    connect(m_buildMine, SIGNAL(clicked(bool)), this, SLOT(buildMine(bool)));

    // Send a ship from the planet which is currently choosen
    QPushButton* m_sendShips = ui->SendShip;
    connect(m_sendShips, SIGNAL(clicked(bool)), this, SLOT(sendShips(bool)));

    QPushButton* m_exit = ui->ExitGame;
    connect(m_exit, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));

    currentPlanet = -1;

    currentYear = 2019;
    QString qyear = QString::fromUtf8("Year: ");
    ui->Date->setText(qyear + QString::number(currentYear));

    ui->PlanetInfo->setVisible(false);

    connect(m_model.get(), SIGNAL(updateInfo()), this, SLOT(updateAllInfo()));

    // event is triggert as soon as planets a available in DataModel
    connect(m_model.get(), SIGNAL(initMap()), this, SLOT(initPlanets()));
}

void MainWindow2D::updateAllInfo() {
    updatePlayerInfo();
    updatePlanetColor();
    if (currentPlanet >= 0) {
        updatePlanetInfo(currentPlanet);
    }
}

void MainWindow2D::resizeEvent(QResizeEvent* event){
    std::cout << "Resizing" << std::endl;
    ui->Map->fitInView(0, 0, scene_width, scene_height, Qt::KeepAspectRatio);
}

MainWindow2D::~MainWindow2D() 
{
    if(ui)
        delete ui;
    if(FighterWindow != NULL)
        delete FighterWindow;
    if (scene)
        delete scene;
}


void MainWindow2D::fight(bool click)
{
    m_model->switchWindow(DataModel::SWITCH);
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
        else if (planets.at(id)->getOwner()==m_model->getEnemyPlayer()){
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
                otherEllipse->myBrush = QBrush(otherpix);
            // TODO Players are now saved in a map with their id
            //      iterate over all players if getOwner() != NULL
            } else if (planets.at(currentPlanet)->getOwner()==m_model->getEnemyPlayer()){
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
        } else if (planets.at(id)->getOwner()==m_model->getEnemyPlayer()){
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
    m_model->endOfRound();

    QGraphicsBlurEffect *a=new QGraphicsBlurEffect;
    a->setBlurHints(QGraphicsBlurEffect::QualityHint);
    ui->centralwidget->setGraphicsEffect(a);
    ui->centralwidget->setEnabled(false);

    //Wenn lang genug warten könnet das hier funktionieren
    // QDialog *pop_up = new QDialog(this);
    // pop_up->setWindowFlags(Qt::Window);
    // pop_up->showFullScreen();
    // pop_up->setAttribute(Qt::WA_TranslucentBackground); 
    // pop_up->setWindowOpacity(0.5);
    // pop_up->setStyleSheet("QDialog{background-color: transparent;}");
    // pop_up->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    // pop_up->setLayout(new QVBoxLayout);
    // QLabel *transparentLabel = new QLabel("Bitte Warten...");
    // transparentLabel->setAlignment(Qt::AlignCenter);
    // transparentLabel->setStyleSheet("QLabel{color: white; font: 30pt bold; background-color: transparent;}");
    // pop_up->layout()->addWidget(transparentLabel);
    // pop_up->show();

    //Hier abwarten ob anderer auch fertig ist

    //pop_up->close();
    ui->centralwidget->setEnabled(true);
    a->setEnabled(false);

    currentYear++;
    QString qyear = QString::fromUtf8("Year: ");
    ui->Date->setText(qyear + QString::number(currentYear));

    updatePlayerInfo();
    updatePlanetInfo(currentPlanet);
    updatePlanetColor();

    //Anzeige der aktuellen Flüge löschen
    std::list<std::pair<int,int>> edges = m_model->getEdges();
    for(std::list<std::pair<int,int>>::iterator it=edges.begin(); it != edges.end(); ++it){
        std::pair<int,int> coordinates = *it;
        int pos_1 = coordinates.first;
        int pos_2 = coordinates.second;
        QGraphicsTextItem *qgti = m_fighterAmount[std::make_pair(pos_1,pos_2)];
        qgti->setPlainText(QString::fromStdString(""));
        qgti->update();
    }

    // TODO wait for response of server, block the window until all players are ready
    emit endround_signal();
}

void MainWindow2D::updatePlanetColor(){
    std::map<int, Planet::Ptr> planets = m_model->getPlanets();
    for(int id = 0; id < (int)planets.size(); id++){
        if(id!=currentPlanet){
            MyEllipse* ellipse = getEllipseById(id);
            if(planets.at(id)->getOwner()==m_model->getSelfPlayer()){
                QPixmap pix("../models/surface/my1.jpg");
                ellipse->myBrush = QBrush(pix);
            }else if (planets.at(id)->getOwner()==m_model->getEnemyPlayer()){
                QPixmap pix("../models/surface/other1.jpg");
                ellipse->myBrush = QBrush(pix);
            } else{
                QPixmap pix("../models/surface/neutral1.jpg");
                ellipse->myBrush = QBrush(pix);  
            }
            ellipse->update();
        }
        QString string = QString::fromStdString(planets.at(id)->getName());
        QGraphicsTextItem *qgti = m_fighterPlanet[id];
        if(planets.at(id)->getShips()>0){
            string = string + QString::fromStdString("\n    ✈:");
            string = string + QString::number(planets.at(id)->getShips());
        }
        qgti->setPlainText(string);
        qgti->update();
    }
}


void MainWindow2D::colonize(bool click)
{
    // now you may end your round
    ui->NextRound->setVisible(true);

    Planet::Ptr p = m_model->getPlanetFromId(currentPlanet);

    m_model->setStartPlanet(p);

    ui->Colonize->setVisible(false);
    MyEllipse* otherEllipse = getEllipseById(currentPlanet);
    QPixmap otherpix("../models/surface/my2.jpg");
    otherEllipse->myBrush = QBrush(otherpix);
    otherEllipse->update();

    // Aktualisiere Informationen
    ui->Info->setText(QString::fromStdString(p->getOwner()->getPlayerName()));
    ui->ShipNumber->setText(QString::number(p->getShips()));
    updatePlanetInfo(currentPlanet);
    updatePlayerInfo();

    QString string = QString::fromStdString(p->getName());
    QGraphicsTextItem *qgti = m_fighterPlanet[currentPlanet];
    if(p->getShips()>0){
        string = string + QString::fromStdString("\n    ✈:");
        string = string + QString::number(p->getShips());
    }
    qgti->setPlainText(string);
    qgti->update();
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
        p->incShipsOrdered();
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
    
    std::string planetname = ui->DestionationPlanet->currentText().toStdString();
    Planet::Ptr to = m_model->getPlanetFromName(planetname);
    Planet::Ptr from = m_model->getPlanetFromId(currentPlanet);
    int ships = ui->SendShipNumber->currentText().toInt();
    m_model->moveShips(from, to, ships);
    updatePlanetInfo(currentPlanet);
    updatePlayerInfo();
    if(ships >0)
    {
        QString string = QString::fromStdString(from->getName());
        QGraphicsTextItem *qgti = m_fighterPlanet[currentPlanet];
        if(from->getShips()>0){
            string = string + QString::fromStdString("\n    ✈:");
            string = string + QString::number(from->getShips());
        }
        qgti->setPlainText(string);
        qgti->update();

    //Flüge an Kanten hinzufügen
        int pos_1 = currentPlanet;
        int pos_2 = m_model->getIDFromPlanetName(planetname);
        if(pos_1<=pos_2){
            QGraphicsTextItem *qgti = m_fighterAmount[std::make_pair(pos_1,pos_2)];
            QString qs = qgti->toPlainText();
            std::string s = qs.toStdString();
            int fighter = atoi(s.c_str());
            fighter+=ships;
            std::cout<<fighter<<std::endl;
            std::string ships_string = std::to_string(fighter);
            qgti->setPlainText(QString::fromStdString(ships_string));
            qgti->update();
        }else{
            QGraphicsTextItem *qgti = m_fighterAmount[std::make_pair(pos_2,pos_1)];
            QString qs = qgti->toPlainText();
            std::string s = qs.toStdString();
            int fighter = atoi(s.c_str());
            fighter+=ships;
            std::cout<<fighter<<std::endl;
            std::string ships_string = std::to_string(fighter);
            qgti->setPlainText(QString::fromStdString(ships_string));
            qgti->update();
        }
    }
}

void MainWindow2D::exitGame(bool click)
{
    m_model->switchWindow(DataModel::END);
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
        ui->MineOrdersLabel->setVisible(false);
        ui->MineOrdersValue->setVisible(false);
        ui->ShipOrdersLabel->setVisible(false);
        ui->ShipOrdersValue->setVisible(false);
    } else {
        // Enable entsprechende Felder, wenn Planet besessen wird
        if (p->getShips() > 0)
        {
            ui->SendShip->setVisible(true);
            ui->SendShipNumber->setVisible(true);
            ui->DestionationPlanet->setVisible(true);
        } else {
            ui->SendShip->setVisible(false);
            ui->SendShipNumber->setVisible(false);
            ui->DestionationPlanet->setVisible(false);
        }
        
        ui->ShipOrdersLabel->setVisible(true);
        ui->ShipOrdersValue->setVisible(true);
        ui->MineOrdersLabel->setVisible(true);
        ui->MineOrdersValue->setVisible(true);

        /* Schiffe und Minen können nur mit genügend Rubinen gekauft werden */
        if (m_model->getSelfPlayer()->getRubin() < m_model->getShipCost())
        {
            ui->BuildShip->setVisible(false);
        } else {
            ui->BuildShip->setVisible(true);
        }
        if (m_model->getSelfPlayer()->getRubin() < m_model->getMineCost())
        {
            ui->BuildMine->setVisible(false);
        } else {
            ui->BuildMine->setVisible(true);
        }


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

    QString mineText = QString::number(p->getMinesBuild()) + " / " + QString::number(p->getMines());
    ui->MineNumber->setText(mineText);
    // Verstecke den Button, wenn die max. Minenanzahl erreicht ist
    if (p->getMinesBuild() + p->getMinesHidden() == p->getMines())
    {
        ui->BuildMine->setVisible(false);
    }

    ui->ShipNumber->setText(QString::number(p->getShips()));
    if (p->getOwner() == NULL)
    {
        ui->Info->setText("Niemand besitzt diesen Planeten!");
    }
    else
    {
        ui->Info->setText(QString::fromStdString(p->getOwner()->getPlayerName()));
    }

    ui->MineOrdersValue->setText(QString::number(p->getMinesHidden()));
    ui->ShipOrdersValue->setText(QString::number(p->getShipsOrdered()));
}

void MainWindow2D::initPlanets()
{
    if (!map_created)
    {
        ui->NextRound->setVisible(false);
        QPen outlinePenHighlight(Qt::gray);
        outlinePenHighlight.setWidth(1);

        std::map<int, Planet::Ptr> planets = m_model->getPlanets();

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
            m_fighterPlanet[i] = io;
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
            //Für die anzahl der zurzeit gesendeten Flüge die unterwegs sind
            QGraphicsTextItem *qgti = new QGraphicsTextItem;
            qgti->setPos((p1->getPosX() + p2->getPosX())/2,(p1->getPosY() + p2->getPosY())/2);
            qgti->setPlainText(QString::fromStdString(""));
            qgti->setDefaultTextColor(Qt::white);
            qgti->setFont(QFont("Helvetica",5));
            qgti->setZValue(1);
            scene->addItem(qgti);
            if(pos_1<=pos_2){
                m_fighterAmount[std::make_pair(pos_1,pos_2)]=qgti;
            }else{
                m_fighterAmount[std::make_pair(pos_2,pos_1)]=qgti;
            }
        }
        map_created = true;
    } else {
        std::cout << "Error MainWindow2D initPlanets: Versucht Map ein weiteres Mal zu zeichnen!" << std::endl;
    }
    //ui->Map->update(0, 0, scene_width*2, scene_height*2);
}

void MainWindow2D::setMapSize(int width, int height)
{
    scene_width = width;
    scene_height = height;
}

}
