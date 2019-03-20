/*
 * MainWindow2D.hpp
 */

#ifndef __MAINWINDOW2D_HPP__
#define __MAINWINDOW2D_HPP__

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>

#include "../build/ui_MainWindow2D.h"
#include "view/MainWindow.hpp"
#include "view2D/MyEllipse.hpp"

#include "datamodel/DataModel.hpp"


namespace strategy
{

/**
 * @brief   
 *
 */
class MainWindow2D : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Main Window object
     *
     */
     MainWindow2D(DataModel *model, QWidget* parent = NULL);

    /**
     * @brief Destroys the Main Window object
     *
     */
    ~MainWindow2D();


    /// Returns the width of the window
    //int width();

    /// Returns the height of the windows
    //int height();
public slots:
    void fight(bool click);

    void choose_planet();

private:

    /// QT UI of the window
    Ui::MainWindow2D* ui;

    QGraphicsScene* scene;

    asteroids::MainWindow* FighterWindow;

    DataModel* model;

};


}

#endif