#include "view2D/EndWindow.hpp"
#include "view2D/MainWindow2D.hpp"
#include "datamodel/DataModel.hpp"

#include <QPixmap>


namespace strategy {

EndWindow::EndWindow(DataModel::Ptr model, QWidget* parent) : 
    QMainWindow(parent), ui(new Ui::EndWindow())
{
    m_model = model;
    m_model->addWidget(DataModel::END,this);
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->Image->setScene(scene);
   
    // connect Button with QApplication::quit()
    QPushButton* exitButton = ui->ExitGame;
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitGame(bool)));

    // get signal when game has ended, update the windows content
    connect(m_model.get(), SIGNAL(endOfGame()), this, SLOT(activate()));
}

EndWindow::~EndWindow() {
    if (ui) {
        delete ui;
    }
}

void EndWindow::activate()
{
    // DataModel::endOfGame() has to be emitted somewhere so this method is executed
    QPixmap* pixmap = new QPixmap();
    switch(m_model->getResult())
    {
        case 1:
            ui->ResultLabel->setText("Victory!");
            pixmap->load("../models/victory.jpg");
            scene->addPixmap(*pixmap);
            ui->Image->fitInView(0, 0, pixmap->width(), pixmap->height(), Qt::KeepAspectRatio);
            break;
        case 2:
            ui->ResultLabel->setText("Defeat!");
            pixmap->load("../models/defeat.jpg");
            scene->addPixmap(*pixmap);
            ui->Image->fitInView(0, 0, pixmap->width(), pixmap->height(), Qt::KeepAspectRatio);
            break;
        default:
            ui->ResultLabel->setText("Error!");
            pixmap->load("../models/defeat.jpg");
            scene->addPixmap(*pixmap);
            ui->Image->fitInView(0, 0, pixmap->width(), pixmap->height(), Qt::KeepAspectRatio);
            break;
    }
}


void EndWindow::exitGame(bool clicked)
{
    QCoreApplication::quit();
}

}
