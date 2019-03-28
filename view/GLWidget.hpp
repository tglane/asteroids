/*
 *  GLWidget.hpp
 *
 *  Created on: Jan. 14 2019
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __GLWIDGET_HPP__
#define __GLWIDGET_HPP__

#include <string>

#include "util/gl_includes.h"
#include <QOpenGLWidget>
#include <QWidget>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QtCore/QTime>

#include "view/Camera.hpp"
#include "rendering/SpaceCraft.hpp"
#include "rendering/Skybox.hpp"
#include "util/AsteroidField.hpp"
#include "physics/PhysicsEngine.hpp"
#include "network/client/udpclient.hpp"
#include "view/Controller.hpp"

using namespace asteroids;
using std::shared_ptr;

/**
 * @brief   implements the OpenGL View in a QT Widget
 *
 */
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget* parent = NULL);

    /// Parses a level file and loads all supported objects
    void setLevelFile(const std::string& file);

    /// Handles keyboard input and updates the physics engine
    void step(map<Qt::Key, bool>& keyStates);

    void drawHealth(QPainter& painter, int totalHealthPlayer, int totalHealthEnemy);

    void drawMinimap(QPainter& painter, Hittable::Ptr player, Hittable::Ptr enemy);

    void setClient(udpclient::Ptr client);

    PhysicsEngine::Ptr getPhysicsEngine() { return m_physicsEngine; }

    Camera::Ptr getCamera() { return m_camera; }

    SpaceCraft::Ptr getEnemy() { return m_enemy; }

    void reset();

    ~GLWidget() { std::cout << "GLWidget deconstruct" << std::endl; }

protected:

    /// Init OpenGL
    virtual void initializeGL() override;

    /// Render scene
    virtual void paintGL() override;

    /// Handle new window dimenions
    virtual void resizeGL(int w, int h) override;

private:

    /// Name of the given level file
    string                      m_levelFile;

    /// The virtual camera
    Camera::Ptr					m_camera;

    /// A pointer to the Actor
    SpaceCraft::Ptr  	        m_enemy;

    /// A skybox for the scene
    Skybox::Ptr			        m_skybox;

    /// Pointer to the asteroid field
    AsteroidField::Ptr          m_asteroidField;
    
    /// Physics 
    PhysicsEngine::Ptr          m_physicsEngine;

    /// timer for correct speed with low fps
    QTime                       m_fpsTimer;

    QTime                       m_startTimer;

    Controller                  m_controller;

    /// Udpclient to send pos/mov/rot to the server
    udpclient::Ptr               m_client;

    bool                        m_started;

    bool                        m_gameOver;

    bool                        m_outOfBound;

    bool                        m_useGamepad;

    QPixmap                     m_cockpit;

    QPixmap                     m_playerHeart;

    QPixmap                     m_enemyHeart;

    QPixmap                     m_emptyHeart;

    static bool open_gl;
};

#endif
