#include "view2D/EndWindow.hpp"
#include "view2D/MainWindow2D.hpp"


namespace strategy {

EndWindow::EndWindow(bool victory, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::EndWindow())
{
    ui->setupUi(this);

    ui->ResultLabel->setStyleSheet("QLabel { color: white }");
    if (victory)
    {
        ui->ResultLabel->setText("Victory!");
        QPixmap bkgnd("../models/victory.jpg");
    } else {
        ui->ResultLabel->setText("Defeat!");
        QPixmap bkgnd("../models/defeat.jpg");

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
