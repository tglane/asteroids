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
    QPushButton* startfight = ui->StartFightButton;

    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(startfight, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(startfight, DataModel::MAIN3D);

    connect(signalMapper, SIGNAL(mapped(int)), m_model.get(), SLOT(switchWindow(int)));
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
    ui->Info->setText(QString::fromStdString("Fight for planet " + planetname));
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
    ui->Info->setText(QString::fromStdString("Fight for planet ended" + planetname));
    ui->PlayerOne->setText(QString::fromStdString(player1));
    ui->PlayerTwo->setText(QString::fromStdString(player2));
    ui->ShipBeforeOne->setText(QString::number(ship1));
    ui->ShipBeforeTwo->setText(QString::number(ship2));
    ui->ShipAfterOne->setVisible(true);
    ui->ShipAfterTwo->setVisible(true);
    ui->ShipAfterOne->setText(QString::number(ship_after1));
    ui->ShipAfterTwo->setText(QString::number(ship_after2));
}


SwitchingWindowInfo::~SwitchingWindowInfo() {
    if (ui)
        delete ui;
}



}