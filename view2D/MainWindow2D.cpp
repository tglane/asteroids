/*
 * MainWindow2D.cpp
 */

#include "MainWindow2D.hpp"



namespace strategy {

MainWindow2D::MainWindow2D(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2D())
{
    // Setup user interface
    ui->setupUi(this);
       scene = new QGraphicsScene(this);
       ui->graphicsView->setScene(scene);

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

}

MainWindow2D::~MainWindow2D() 
{
    delete ui;
}

}