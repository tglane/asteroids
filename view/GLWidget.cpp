
#include "GLWidget.hpp"
#include "io/LevelParser.hpp"
#include "io/TextureFactory.hpp"
#include <QMouseEvent>

#include <SDL2/SDL.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      m_camera(Vector3f(0.0f, 0.0f, -700.0f), 0.05f, 5.0f),
      m_rotationSpeed(0.02),
      m_moveSpeed(1.0)
{
}

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
    LevelParser lp(m_levelFile, m_actor, m_skybox, m_asteroidField);

    // Setup physics
    m_physicsEngine = make_shared<PhysicsEngine>();

    // Add asteroids to physics engine
    std::list<Asteroid::Ptr> asteroids;
    m_asteroidField->getAsteroids(asteroids);
    for (auto it = asteroids.begin(); it != asteroids.end(); it++)
    {
        PhysicalObject::Ptr p = std::static_pointer_cast<PhysicalObject>(*it);
        m_physicsEngine->addDestroyable(p);
    }
}

void GLWidget::paintGL()
{
    // Clear bg color and enable depth test (z-Buffer)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera.apply();

    // Render stuff
    m_skybox->render(m_camera);

    // Render all physical objects
    m_physicsEngine->render();

    m_actor->render();
}

void GLWidget::step(map<Qt::Key, bool>& keyStates)
{
    // Get keyboard states and handle model movement
    m_physicsEngine->process();

    if (keyStates[Qt::Key_Up])
    {
        m_actor->rotate(Transformable::YAW_LEFT, m_rotationSpeed);
    }
    if (keyStates[Qt::Key_Down])
    {
        m_actor->rotate(Transformable::YAW_RIGHT, m_rotationSpeed);
    }
       if (keyStates[Qt::Key_Left])
    {
        m_actor->rotate(Transformable::ROLL_LEFT, m_rotationSpeed);
    }
    if (keyStates[Qt::Key_Right])
    {
        m_actor->rotate(Transformable::ROLL_RIGHT, m_rotationSpeed);
    }
   
    if (keyStates[Qt::Key_W])
    {
        m_actor->move(Transformable::FORWARD, m_moveSpeed);
    }
    if (keyStates[Qt::Key_S])
    {
        m_actor->move(Transformable::BACKWARD, m_moveSpeed);
    }
    if (keyStates[Qt::Key_A])
    {
        m_actor->move(Transformable::STRAFE_LEFT, m_moveSpeed);
    }
    if (keyStates[Qt::Key_D])
    {
        m_actor->move(Transformable::STRAFE_RIGHT, m_moveSpeed);
    }

    // Add a bullet to physics engine
    if(keyStates[Qt::Key_Space])
    {
        Bullet::Ptr bullet = make_shared<Bullet>(Bullet(m_actor->getPosition(), m_actor->getDirection()));
        m_physicsEngine->addBullet(bullet);
    }

    // Trigger update, i.e., redraw via paintGL()
    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    bool l_pressed = (event->buttons() & Qt::LeftButton) != 0;
    bool r_pressed = (event->buttons() & Qt::RightButton) != 0;

    QPoint delta = event->pos() - m_mousePos;
    m_mousePos = event->pos();

    // Handle motion for pressed L button while R is not
    // pressed
    if (l_pressed & !r_pressed)
    {
        if (delta.x() > -3)
        {
            m_camera.turn(Camera::RIGHT);
        }
        if (delta.x() < 3)
        {
            m_camera.turn(Camera::LEFT);
        }
        if (delta.y() > 3)
        {
            m_camera.turn(Camera::UP);
        }
        if (delta.y() < -3)
        {
            m_camera.turn(Camera::DOWN);
        }
    }

    // Handle motion for pressed R button while L is not
    // pressed
    if (r_pressed & !l_pressed)
    {
        if (delta.x() > 3)
        {
            m_camera.move(Camera::LEFT);
        }
        if (delta.x() < -3)
        {
            m_camera.move(Camera::RIGHT);
        }
        if (delta.y() > 3)
        {
            m_camera.move(Camera::FORWARD);
        }
        if (delta.y() < -3)
        {
            m_camera.move(Camera::BACKWARD);
        }
    }
}

void GLWidget::resizeGL(int w, int h)
{
    float ratio = width() * 1.0 / height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(45, ratio, 1, 10000);

    glMatrixMode(GL_MODELVIEW);
}
