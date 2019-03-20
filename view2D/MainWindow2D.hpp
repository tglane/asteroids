/*
 * MainWindow2D.hpp
 */

#ifndef __MAINWINDOW2D_HPP__
#define __MAINWINDOW2D_HPP__

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>

#include "../build/ui_MainWindow2D.h"
#include "view/MainWindow.hpp"

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

    /**
     * @brief   Is called when button NextRound is clicked
     *          Calls the synchronizing function in the data model,
     *          blocks until all players have finished their rounds
     */
    void endOfRound(bool click);

private:

    /// QT UI of the window
    Ui::MainWindow2D* ui;

    QGraphicsScene* scene;

    asteroids::MainWindow* FighterWindow;

    DataModel* model;

};


}

#endif