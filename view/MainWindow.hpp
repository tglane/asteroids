/*
 *  MainWindow.hpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <string>

#include <QMainWindow>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "GLWidget.hpp"

// TODO: Besser!
#include "build/ui_MainWindow.h"

namespace asteroids
{
/**
 * @brief   Represents the main window of the game. This
 *          class contains the main loop, handles all
 *          user input and renders all objects
 *
 */
    class MainWindow : public QMainWindow
    {
    Q_OBJECT

    public:

        /**
         * @brief Construct a new Main Window object
         *
         * @param plyname  A .ply file to render
         */
        MainWindow(const std::string& plyname, QWidget* parent = NULL);

        /**
         * @brief Destroys the Main Window object
         *
         */
        ~MainWindow();

        /// Returns the width of the window
        int width();

        /// Returns the height of the windows
        int height();

        void start_timer() { m_timer->start(1000 / 60.0); }

        Ui::MainWindow* ui;

    public Q_SLOTS:
        /// Handle input
        void handleInput();

    protected:

        /// Called if a key was pressed
        virtual void keyPressEvent(QKeyEvent* event) override;

        /// Calles of a key was released
        virtual void keyReleaseEvent(QKeyEvent* event) override;

    private:

        /// QT UI of the window


        /// gl widget
        GLWidget*       m_widget;

        /// map with the keys and their states
        map<Qt::Key, bool>          m_keyStates;

        /// 60 fps timer
        shared_ptr<QTimer>          m_timer;
    };

}

#endif
