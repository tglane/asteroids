#include "view2D/EndWindow.hpp"
#include "view2D/MainWindow2D.hpp"
#include "datamodel/DataModel.hpp"

#include <QPixmap>
#include <QRandomGenerator>


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

EndWindow::~EndWindow() 
{
    if (ui) {
        delete ui;
    }
}

void EndWindow::activate()
{
    QPixmap* pixmap = new QPixmap();
    QRandomGenerator* generator = new QRandomGenerator(QDateTime::currentMSecsSinceEpoch());
    
    scene->clear();

    // randomly loading image depending on win or loss
    switch(m_model->getResult())
    {
        case 1:
            if(generator->generateDouble() > 0.5)
            {
                pixmap->load("../models/victory2.jpg");
            }
            else
            {
                pixmap->load("../models/victory.jpg");
            }
            ui->ResultLabel->setText("Victory!");
            break;
        case 0:
        case 2:
            if(generator->generateDouble() > 0.5)
            {
                pixmap->load("../models/defeat.jpg");
            }
            else
            {
                pixmap->load("../models/defeat2.jpg");
            }
            ui->ResultLabel->setText("Defeat!");
            break;
        default:
            ui->ResultLabel->setText("Error!");
            pixmap->load("../models/defeat.jpg");
            break;
    }
    scene->addPixmap(*pixmap);
    
    // set size of QGraphicsView
    ui->Image->fitInView(0, 0, pixmap->width()/1.5, pixmap->height()/1.5, Qt::KeepAspectRatio);
}

void EndWindow::exitGame(bool clicked)
{
    QCoreApplication::quit();
}

}
