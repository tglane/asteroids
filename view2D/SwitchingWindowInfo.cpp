#include "view2D/SwitchingWindowInfo.hpp"

#include <QSignalMapper>

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

SwitchingWindowInfo::~SwitchingWindowInfo() {}



}