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

    // Map mit den Planeten-Objekten aus dem DataModel
    std::map<int, Planet*> planets = model->getPlanets();

    // Map für die Elipsen-Objekten im QGraphicsScene
    std::map<int, MyEllipse*> view_planets;

    //Erstelle die Elipsen und füge sie in die Map und in die QGraphicsScene ein 
    for(int i = 0; i < planets.size(); i++){
        Planet *p = planets.at(i);
        view_planets[i] = new MyEllipse(p->getPosX(), p->getPosY());
        scene->addItem(view_planets[i]);
        QVariant ellipse_ID(i);
        //view_planets[i]->setData(1, ellipse_ID);
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

void MainWindow2D::choose_planet()
{
    std::cout << "Planet angeklickt" << std::endl;
}

}