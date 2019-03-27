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

#include "../cmake-build-debug/ui_MainWindow2D.h"
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
     MainWindow2D(DataModel::Ptr model, QWidget* parent = NULL);

    /**
     * @brief Destroys the Main Window object
     *
     */
    ~MainWindow2D();

    void updatePlanetInfo(int id);
    
    void initMap();

public slots:

    void updateAllInfo();
    void updatePlayerInfo();

    /**
     * @brief   Switches to the starting window
     */
    void fight(bool click);

    /**
     * @brief   Fills in the information about a planet 
     */
    void choose_planet(int id);

    /**
     * @brief   Is called when button NextRound is clicked
     *          Calls the synchronizing function in the data model,
     *          blocks until all players have finished their rounds
     */
    void endOfRound(bool click);

    /**
     * @brief Change the scale if Window schanges the Size
     */
    void resizeEvent(QResizeEvent* event);

    /**
     * @brief   Is called when the colonization button is pressed
     *          Starts an attemp to colonize the start planet
     *          
     */
    void colonize(bool click/*, Planet* p*/);

    /**
     * @brief   Is called when the Ship building Button is pressed
     *          A new ship is accessible on this Planet one round later
     */
    void buildShip(bool click);

    /**
     * @brief   Builds a mine on a planet if it doesn't already have one
     */
    void buildMine(bool click);

    /**
     * @brief   Send Ships to choosen planet
     */
    void sendShips(bool click);

    /**
     * @brief   Is calle when exit Button is pressed, 
     */
    void exitGame(bool click);

    MyEllipse* getEllipseById(int id);

    void showPlayerName();

    void updatePlanetColor();

signals:
    void endround_signal();

private:

    /// QT UI of the window
    Ui::MainWindow2D* ui;

    QGraphicsScene* scene;

    asteroids::MainWindow* FighterWindow;

    DataModel::Ptr m_model;

    std::map<int, MyEllipse*> view_planets;

    std::map<std::pair<int,int>, QGraphicsTextItem*> m_fighterAmount;

    int currentPlanet;

    int currentYear;
};


}

#endif
