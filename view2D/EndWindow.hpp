#ifndef __ENDWINDOW_HPP__
#define __ENDWINDOW_HPP__

#include <QMainWindow>
#include <QPushButton>

#include "build/ui_EndWindow.h"
#include "datamodel/DataModel.hpp"


namespace strategy
{

/**
 * @brief   A window shown on end of the game
 *
 */
class EndWindow : public QMainWindow
{
    Q_OBJECT

public:
    EndWindow(asteroids::DataModel::Ptr model, QWidget* parent = NULL);

    ~EndWindow();

public slots:

    /**
     * @brief   Is calle when exit Button is pressed, 
     */
    void exitGame(bool click);

private:

    /// QT UI of the window
    Ui::EndWindow* ui;

    asteroids::DataModel::Ptr m_model;

};

}

#endif