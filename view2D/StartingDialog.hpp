#ifndef __STARTINGDIALOG_HPP__
#define __STARTINGDIALOG_HPP__

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <string>

#include "cmake-build-debug/ui_StartingDialog.h"
#include "datamodel/DataModel.hpp"


using asteroids::DataModel;
using std::string;

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
class StartingDialog : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Construct a new Main Window object
     *
     */
    StartingDialog(DataModel::Ptr model, QWidget* parent = NULL);

    /**
     * @brief Destroys the Main Window object
     *
     */
    ~StartingDialog();


    /// Returns the width of the window
    //int width();

    /// Returns the height of the windows
    //int height();
public slots:

    /**
     * @brief   Is calle when exit Button is pressed, 
     */
    void exitGame(bool click);

    void startGame(bool click);

signals:
    void connect_to_server(string, string);

private:

    /// QT UI of the window
    Ui::StartingDialog* ui;

    // The model which manages the round based data
    DataModel::Ptr m_model;
};


}

#endif