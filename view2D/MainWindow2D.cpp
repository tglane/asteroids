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

}

MainWindow2D::~MainWindow2D() 
{
    delete ui;
}

}