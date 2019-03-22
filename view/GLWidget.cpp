#include "GLWidget.hpp"
#include "io/LevelParser.hpp"
#include "io/TextureFactory.hpp"
#include <QMouseEvent>
#include <SDL2/SDL.h>
#include <QtGui/QPainter>
//#include <SOIL.h>

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent), m_gameOver(false) {}

void GLWidget::setLevelFile(const std::string& file)
{
    m_levelFile = file;
}

void GLWidget::initializeGL()
{
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    // Init OpenGL projection matrix
    glClearColor(0.0, 0.0, 0.0, 1.0);
    float ratio = width() * 1.0 / height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(45, ratio, 1, 10000);

    // Enter model view mode
    glMatrixMode(GL_MODELVIEW);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    // Setup two light sources
    float light0_position[4];
    float light0_ambient[4];
    float light0_diffuse[4];

    float light1_position[4];
    float light1_ambient[4];
    float light1_diffuse[4];

    light0_position[0] =   1.0f;
    light0_ambient[0] = 0.8f;
    light0_diffuse[0] = 0.8f;
    light0_position[1] =   1.0f;
    light0_ambient[1] = 0.8f;
    light0_diffuse[1] = 0.8f;
    light0_position[2] =   0.0f;
    light0_ambient[2] = 0.8f;
    light0_diffuse[2] = 0.8f;
    light0_position[3] =   1.0f;
    light0_ambient[3] = 0.1f;
    light0_diffuse[3] = 1.0f;

    light1_position[0] =   0.0f;
    light1_ambient[0] = 0.1f;
    light1_diffuse[0] = 0.5f;
    light1_position[1] =  -1.0f;
    light1_ambient[1] = 0.1f;
    light1_diffuse[1] = 0.5f;
    light1_position[2] =   0.0f;
    light1_ambient[2] = 0.1f;
    light1_diffuse[2] = 0.5f;
    light1_position[3] =   1.0f;
    light1_ambient[3] = 1.0f;
    light1_diffuse[3] = 1.0f;

    // Light 1
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    // Light 2
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Enable z buffer and gouroud shading
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel (GL_SMOOTH);

    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL,
    // but most video cards whould be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Load level
    LevelParser lp(m_levelFile, m_enemy, m_skybox, m_asteroidField);
    m_enemy->fixArrow();
    m_enemy->setId(1);
    m_enemy->setHealth(10);

    // Setup physics
    m_physicsEngine = make_shared<PhysicsEngine>();

    m_camera = make_shared<Camera>();
    m_camera->setId(0);
    m_camera->setHealth(10);
    m_camera->setPosition(Vector3f(2500, 0, 0));
    m_camera->setXAxis(Vector3f(-1, 0, 0));
    m_camera->setYAxis(Vector3f(0, -1, 0));

    Hittable::Ptr player_ptr = std::static_pointer_cast<Hittable>(m_camera);
    m_physicsEngine->addHittable(player_ptr);
    Hittable::Ptr enemy_ptr = std::static_pointer_cast<Hittable>(m_enemy);
    m_physicsEngine->addHittable(enemy_ptr);

    // Add asteroids to physics engine
    std::list<Asteroid::Ptr> asteroids;
    m_asteroidField->getAsteroids(asteroids);
    for (auto it = asteroids.begin(); it != asteroids.end(); it++)
    {
        PhysicalObject::Ptr p = std::static_pointer_cast<PhysicalObject>(*it);
        m_physicsEngine->addDestroyable(p);
    }

    m_useGamepad = m_controller.gamepadAvailable();
    m_outOfBound = false;

    m_timer.start();
}

void GLWidget::paintGL()
{
    // Clear bg color and enable depth test (z-Buffer)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_camera->apply();

    // Render stuff
    m_skybox->render();

    // Render all physical objects
    m_physicsEngine->render();

    if (m_enemy->getHealth() > 0)
    {
        m_enemy->render();
    }

    glDisable(GL_DEPTH_TEST);

    QPainter painter(this);
    QPixmap hud("../models/cockpit.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), hud);

    drawHealth(painter, m_camera->getHealth(), m_enemy->getHealth());

    if (m_gameOver)
    {
        QPixmap won("../models/won.png");
        QPixmap lost("../models/lost.png");
        painter.drawPixmap(0, 0, this->width(), this->height(), (m_camera->getHealth() > 0) ? won : lost);
    }

    if (m_outOfBound && !m_gameOver)
    {
        QPixmap turnWarning("../models/turn_warning.png");
        painter.drawPixmap(0, 0, this->width(), this->height(), turnWarning);
    }

    // paint with OpenGL
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    glOrtho(0.0, this->width(), this->height(), 0.0, -1.0, 10.0);
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glLoadIdentity();
//    glDisable(GL_CULL_FACE);
//    glDisable(GL_DEPTH_TEST);
//    glDepthMask(GL_FALSE);
//    glClear(GL_DEPTH_BUFFER_BIT);
//
//    int width = this->width();
//    int height = this->height();
//    unsigned char* image = SOIL_load_image("../models/cockpit.png", &width, &height, 0, SOIL_LOAD_RGB);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    glColor3f(1.0, 1.0, 1.0);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0, 1.0);
//    glVertex2f(0.0, this->height() * 0.7);
//    glTexCoord2f(1.0, 1.0);
//    glVertex2f(this->width(), this->height() * 0.7);
//    glTexCoord2f(1.0, 1.0);
//    glVertex2f(this->width(), this->height());
//    glTexCoord2f(0.0, 0.0);
//    glVertex2f(0.0, this->height());
//    glEnd();
//
////     Making sure we can render 3d again
//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
//    glEnable(GL_DEPTH_TEST);
//    glDepthMask(GL_TRUE);
}

void GLWidget::step(map<Qt::Key, bool>& keyStates)
{
    int elapsed_time = m_timer.restart();

    // Get keyboard states and handle model movement
    m_gameOver = m_physicsEngine->process(elapsed_time) | m_gameOver;

    if (!m_gameOver) {
        Hittable::Ptr player_ptr = std::static_pointer_cast<Hittable>(m_camera);

        if (m_useGamepad)
        {
            m_controller.gamepadControl(player_ptr, m_physicsEngine, elapsed_time);
        }
        else
        {
            m_controller.keyControl(keyStates, player_ptr, m_physicsEngine, elapsed_time);
        }

        Vector3f player_pos = m_camera->getPosition();
        if (std::abs(player_pos[0]) > 4500 || std::abs(player_pos[1]) > 4500 || std::abs(player_pos[2]) > 4500)
        {
            m_camera->outOfBound();
            int time = m_camera->getTime();
            if (time > 1000)
            {
                m_camera->restartTimer(time - 1000);
                m_camera->setHealth(m_camera->getHealth() - 1);
                if (m_camera->getHealth() == 0)
                {
                    m_gameOver = true;
                }
            }
            m_outOfBound = true;
        }
        else
        {
            m_camera->inBound();
            m_outOfBound = false;
        }

        Vector3f enemy_pos = m_enemy->getPosition();
        if (std::abs(enemy_pos[0]) > 4500 || std::abs(enemy_pos[1]) > 4500 || std::abs(enemy_pos[2]) > 4500)
        {
            m_enemy->outOfBound();
            int time = m_enemy->getTime();
            if (time > 1000)
            {
                m_enemy->restartTimer(time - 1000);
                m_enemy->setHealth(m_enemy->getHealth() - 1);
                if (m_enemy->getHealth() == 0)
                {
                    m_gameOver = true;
                }
            }
        }
        else
        {
            m_enemy->inBound();
        }
    }
    // Trigger update, i.e., redraw via paintGL()
    this->update();
}

void GLWidget::drawHealth(QPainter& painter, int healthPlayer, int healthEnemy)
{
    float size = this->width() / 30.0f;
    float gap = 0.1;
    int height = (int) (size - 2 * size * gap);
    int width = (int) (height * 1.1);

    QPixmap playerHeart("../models/player_heart.png");
    QPixmap enemyHeart("../models/enemy_heart.png");
    QPixmap emptyHeart("../models/empty_heart.png");

    for (int i = 0; i < 10; i++)
    {
        painter.drawPixmap((int) (size * i + size * gap), (int) (size * gap), width, height, (i < healthPlayer) ? playerHeart : emptyHeart);
        painter.drawPixmap((int) (this->width() - size - (size * i + size * gap)), (int) (size * gap), width, height, (i < healthEnemy) ? enemyHeart : emptyHeart);
    }
}

void GLWidget::resizeGL(int w, int h)
{
    float ratio = (float) width() / height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(45, ratio, 1, 10000);
    glMatrixMode(GL_MODELVIEW);
}
