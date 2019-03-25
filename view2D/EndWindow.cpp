#include "view2D/EndWindow.hpp"
#include "view2D/MainWindow2D.hpp"


namespace strategy {

EndWindow::EndWindow(DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::EndWindow())
{
    m_model = model;
    ui->setupUi(this);

    ui->ResultLabel->setStyleSheet("QLabel { color: white }");
    QPixmap bkgnd;
    switch(m_model->getResult())
    {
        case 1:
            ui->ResultLabel->setText("Victory!");
            bkgnd = QPixmap("../models/victory.jpg");
        case 2:
            ui->ResultLabel->setText("Defeat!");
            bkgnd = QPixmap("../models/defeat.jpg");
        default:
            ui->ResultLabel->setText("Error!");
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
