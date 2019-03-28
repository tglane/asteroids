#include "view2D/SwitchingWindowInfo.hpp"

#include <QSignalMapper>
#include <QDesktopWidget>

namespace strategy{

SwitchingWindowInfo::SwitchingWindowInfo(DataModel::Ptr model, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::SwitchingWindowInfo())
{
    m_model = model;

    ui->setupUi(this);

    // connect mit m_model.get() um normalen Pointer aus shared_ptr zu bekommen
    m_startfight = ui->StartFightButton;
}

void SwitchingWindowInfo::resizeEvent(QResizeEvent* event)
{
    QRect rec = QApplication::desktop()->screenGeometry();
    this->resize(rec.width(), rec.height());
    ui->widget->resize(rec.width(), rec.height());
}


void SwitchingWindowInfo::updateWindow(std::string planetname, std::string player1, std::string player2, 
                int ship1, int ship2)
{
    ui->Info->setText(QString::fromStdString("Fight for planet " + planetname + "."));
    ui->PlayerOne->setText(QString::fromStdString(player1));
    ui->PlayerTwo->setText(QString::fromStdString(player2));
    ui->ShipBeforeOne->setText(QString::number(ship1));
    ui->ShipBeforeTwo->setText(QString::number(ship2));
    ui->ShipAfterOne->setVisible(false);
    ui->ShipAfterTwo->setVisible(false);
}

void SwitchingWindowInfo::updateWindow(std::string planetname, std::string player1, std::string player2, 
                int ship1, int ship2, int ship_after1, int ship_after2)
{
    ui->Info->setText(QString::fromStdString("Fight for planet " + planetname + " ended."));
    ui->PlayerOne->setText(QString::fromStdString(player1));
    ui->PlayerTwo->setText(QString::fromStdString(player2));
    ui->ShipBeforeOne->setText(QString::number(ship1));
    ui->ShipBeforeTwo->setText(QString::number(ship2));
    ui->ShipAfterOne->setText(QString::number(ship_after1));
    ui->ShipAfterTwo->setText(QString::number(ship_after2));
    ui->ShipAfterOne->setVisible(true);
    ui->ShipAfterTwo->setVisible(true);
}


SwitchingWindowInfo::~SwitchingWindowInfo() {
    if (ui)
        delete ui;
}



}