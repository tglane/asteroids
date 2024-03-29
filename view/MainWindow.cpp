/*
 *  MainWindow.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "MainWindow.hpp"
#include "io/LevelParser.hpp"
#include "io/TextureFactory.hpp"

#include <iostream>
#include <QTimer>
#include <QKeyEvent>
#include <QPixmap>
#include <QLabel>

namespace asteroids
{

    MainWindow::MainWindow(const std::string& file, QWidget* parent) :
            QMainWindow(parent),
            ui(new Ui::MainWindow()),
            m_timer(new QTimer())

    {
        // Setup user interface
        ui->setupUi(this);
        m_widget = ui->openGLWidget;

        // Set level
        m_widget->setLevelFile(file);

        // Create a timer object to trigger the main loop
        connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(handleInput()));

    }

    int MainWindow::width()
    {
        return m_widget->width();
    }

    int MainWindow::height()
    {
        return m_widget->height();
    }

    void MainWindow::handleInput()
    {
        m_widget->step(m_keyStates);
    }

    void MainWindow::keyPressEvent(QKeyEvent* event)
    {
        // Save which key was pressed
        m_keyStates[(Qt::Key)event->key()] = true;
    }

    void MainWindow::keyReleaseEvent(QKeyEvent* event)
    {
        // Save which key was released
        m_keyStates[(Qt::Key)event->key()] = false;
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

} // namespace asteroids
