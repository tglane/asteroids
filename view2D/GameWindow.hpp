#ifndef __GAMEWINDOW_HPP__
#define __GAMEWINDOW_HPP__

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QMediaPlayer>

#include "build/ui_GameWindow.h"
#include "datamodel/DataModel.hpp"


using asteroids::DataModel;

namespace strategy
{

/**
 * @brief   A window shown on start of the game
 *          Handles:
 *              Connection to the server
 *              Choosing players name
 *              ... 
 *
 */
class GameWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Main Window object
     *
     */
    GameWindow(DataModel::Ptr model, QWidget* parent = NULL);

    /**
     * @brief Destroys the Main Window object
     *
     */
    ~GameWindow();

    QStackedWidget* content();

    /// Returns the width of the window
    //int width();

    /// Returns the height of the windows
    //int height();
signals:
    void play();

    void stop();

    void pause();

    

private:

    /// QT UI of the window
    Ui::GameWindow* ui;

    // The model which manages the round based data
    DataModel::Ptr m_model;

    QMediaPlayer* m_mediaplayer;
};


}

#endif