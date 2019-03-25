#include "view2D/EndWindow.hpp"

namespace strategy {

EndWindow::EndWindow(asteroids::DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::EndWindow())
{
    ui->setupUi(this);

    ui->ResultLabel->setStyleSheet("QLabel { color: white }");
    QPixmap bkgnd;
    if (true)
    {
        ui->ResultLabel->setText("Victory!");
        bkgnd = QPixmap("../models/victory.jpg");
    } else {
        ui->ResultLabel->setText("Defeat!");
        bkgnd = QPixmap("../models/defeat.jpg");

    }

    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

   
    QPushButton* exitButton = ui->ExitGame;
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));
    
}

EndWindow::~EndWindow() {}

void EndWindow::exitGame(bool clicked)
{
    QCoreApplication::quit();
}

}
