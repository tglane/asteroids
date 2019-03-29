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
#include <QtMultimedia/QMediaPlayer>

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

    /**
     * @brief performs one step of the 3d fight at the client
     * @param keyStates map from all keys to their status
     */
    void step(map<Qt::Key, bool>& keyStates);

    /**
     * @brief draws the health bars
     * @param painter painter object used for drawing
     * @param totalHealthPlayer health of the player
     * @param totalHealthEnemy health of the enemy
     */
    void drawHealth(QPainter& painter, int totalHealthPlayer, int totalHealthEnemy);

    /**
     * @brief draws the minimap
     * @param painter painter object used for drawing
     * @param player
     * @param enemy
     */
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

    /// timer used for countdown at the start of the game
    QTime                       m_startTimer;

    /// controller that handles input
    Controller                  m_controller;

    /// Udpclient to send pos/mov/rot to the server
    udpclient::Ptr              m_client;

    /// flag that indicates whether or not the game has started
    bool                        m_started;

    /// flag that indicates whether or not the game is finished
    bool                        m_gameOver;

    /// flag that indicates whether or not the player is out of the bounds of the game space
    bool                        m_outOfBound;

    /// flag that indicated whether or not a gamepad is used to control the game
    bool                        m_useGamepad;

    /// the image of the cockpit
    QPixmap                     m_cockpit;

    /// the image of one heart of the player
    QPixmap                     m_playerHeart;

    /// the image of one heart of the enemy
    QPixmap                     m_enemyHeart;

    /// the image of an empty heart
    QPixmap                     m_emptyHeart;

    /// the sound of the countdown
    QSoundEffect                m_countdownSound;

    /// the sound played when the game is won
    QSoundEffect                m_victorySound;

    /// the sound played when the game is lost
    QSoundEffect                m_defeatSound;

    /// the background music for the fight
    QMediaPlayer*               m_backgroundMusic;

    /// flag that indicated whether or not the sound at the end is playing
    bool                        m_endSoundPlayed;

    static bool open_gl;
};

#endif
