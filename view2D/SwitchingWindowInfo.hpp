#ifndef __SWITCHINGWINDOWINFO_HPP__
#define __SWITCHINGWINDOWINFO_HPP__

#include <QMainWindow>
#include <QPushButton>

#include "build/ui_SwitchingWindowInfo.h"
#include "datamodel/DataModel.hpp"

using asteroids::DataModel;

namespace strategy
{

/**
 * @brief   A window shown on end of the game
 *
 */
class SwitchingWindowInfo : public QMainWindow
{
    Q_OBJECT

public:
    SwitchingWindowInfo(DataModel::Ptr model, QWidget* parent = NULL);

    ~SwitchingWindowInfo();

    void resizeEvent(QResizeEvent* event);

    void updateWindow(std::string planetname, std::string player1, std::string player2, 
                int ship1, int ship2);

    void updateWindow(std::string planetname, std::string player1, std::string player2, 
                int ship1, int ship2, int ship_after1, int ship_after2);

    QPushButton* getButton() { return m_startfight; }

private:

    /// QT UI of the window
    Ui::SwitchingWindowInfo* ui;

    asteroids::DataModel::Ptr m_model;

    QPushButton* m_startfight;
};

}

#endif