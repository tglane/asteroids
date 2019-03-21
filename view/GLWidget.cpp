#include "GLWidget.hpp"
#include "io/LevelParser.hpp"
#include "io/TextureFactory.hpp"
#include <QMouseEvent>
#include <SDL2/SDL.h>
#include <QtGui/QPainter>

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

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

    m_controller = Controller();
}

void GLWidget::paintGL()
{
    // Clear bg color and enable depth test (z-Buffer)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->apply();

    // Render stuff
    m_skybox->render();

    // Render all physical objects
    m_physicsEngine->render();

    m_enemy->render();

    // TODO: tut so noch nicht
    //QPainter qPainter(this);
    //QPixmap hud("../models/cockpit.png");
    //qPainter.drawPixmap(0, 0, this->width(), this->height(), hud);
}

void GLWidget::step(map<Qt::Key, bool>& keyStates)
{
    // Get keyboard states and handle model movement
    m_physicsEngine->process();

    Hittable::Ptr player_ptr = std::static_pointer_cast<Hittable>(m_camera);
    Hittable::Ptr enemy_ptr = std::static_pointer_cast<Hittable>(m_enemy);
    m_controller.keyControl(keyStates, player_ptr, enemy_ptr, m_physicsEngine);

    // Trigger update, i.e., redraw via paintGL()
    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Bei Maus Events passiert nichts
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
