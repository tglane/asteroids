#include "GLWidget.hpp"
#include "io/LevelParser.hpp"

#include "io/TextureFactory.hpp"
#include <QMouseEvent>
#include <SDL2/SDL.h>
#include <QtGui/QPainter>
#include <math.h>

bool GLWidget::open_gl = false;

GLWidget::GLWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    m_started(false),
    m_gameOver(false),
    m_outOfBound(false),
    m_cockpit("../models/cockpit.png"),
    m_playerHeart("../models/player_heart.png"),
    m_enemyHeart("../models/enemy_heart.png"),
    m_emptyHeart("../models/empty_heart.png") {}

void GLWidget::setLevelFile(const std::string& file)
{
    m_levelFile = file;
}

void GLWidget::initializeGL()
{
    // Load level
    LevelParser lp(m_levelFile, m_enemy, m_skybox, m_asteroidField);
    m_enemy->fixArrow();

    // Setup physics
    m_physicsEngine = make_shared<PhysicsEngine>();

    m_camera = make_shared<Camera>();
    m_camera->setPosition(Vector3f(2500, 0, 0));
    m_camera->setXAxis(Vector3f(-1, 0, 0));
    m_camera->setYAxis(Vector3f(0, -1, 0));

    m_useGamepad = m_controller.gamepadAvailable();

    m_fpsTimer.start();
    m_startTimer.start();

    if(open_gl) {
        return;
    }
    open_gl = true;

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


}

void GLWidget::setClient(udpclient::Ptr client) {
    m_client = client;

    if (m_client->get_id() == 42) {
        m_enemy->setId(43 << 24);
    } else {
        m_enemy->setId(42 << 24);
    }

    m_camera->setId(m_client->get_id() << 24);

    m_client->setOtherFighter(m_enemy); //added
    m_client->setPhysicsPtr(m_physicsEngine); //added

    m_physicsEngine->addHittable(m_camera);
    m_physicsEngine->addHittable(m_enemy);
}

void GLWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    
    m_camera->apply();

    // Render stuff
    m_skybox->render();

    // Render all physical objects
    m_physicsEngine->render();

    if (m_enemy->getHealth() > 0)
    {
        // If no Packet recv, move
        if (m_client->getCount() > 0) {
            m_enemy->move();
        }
        m_enemy->render();
        m_client->incCount();

    }

    glDisable(GL_DEPTH_TEST);

    // Draw cockpit
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), m_cockpit);

    drawHealth(painter, m_camera->getHealth(), m_enemy->getHealth());

    drawMinimap(painter, m_camera, m_enemy);

    // Draw start timer
    int time = m_startTimer.elapsed();
    if (time >= 1000 && time < 2000)
    {
        QPixmap cd3("../models/three.png");
        painter.drawPixmap(0, 0, this->width(), this->height(), cd3);
    }
    else if (time >= 2000 && time < 3000)
    {
        QPixmap cd2("../models/two.png");
        painter.drawPixmap(0, 0, this->width(), this->height(), cd2);
    }
    else if (time >= 3000 && time < 4000)
    {
        QPixmap cd1("../models/one.png");
        painter.drawPixmap(0, 0, this->width(), this->height(), cd1);
    }
    else if (time >= 4000 && time < 4500)
    {
        QPixmap cd0("../models/start.png");
        painter.drawPixmap(0, 0, this->width(), this->height(), cd0);
    }

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
}

void GLWidget::reset() {
    m_gameOver = false;
    m_startTimer.restart();
    m_started = false;

    m_physicsEngine->reset_lists();

    m_physicsEngine->addHittable(m_camera);
    m_physicsEngine->addHittable(m_enemy);
}

void GLWidget::step(map<Qt::Key, bool>& keyStates)
{

    int elapsed_time = m_fpsTimer.restart();

    // Get keyboard states and handle model movement

    m_gameOver = m_physicsEngine->process(elapsed_time) || m_gameOver;

    if (!m_started)
    {
        if (m_startTimer.elapsed() >= 4000)
        {
            m_started = true;
        }
    }
    else
    {
        if (!m_gameOver)
        {
            Hittable::Ptr player_ptr = std::static_pointer_cast<Hittable>(m_camera);

            if (m_useGamepad)
            {
                m_controller.gamepadControl(player_ptr, m_physicsEngine, *m_client, elapsed_time);
            }
            else
            {
                m_controller.keyControl(keyStates, player_ptr, m_physicsEngine, *m_client, elapsed_time);
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
            } else
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
    }
    m_client->send_position(m_camera->getPosition(), Vector3f(), m_camera->getXAxis(), m_camera->getYAxis(), m_camera->getZAxis());

    // Trigger update, i.e., redraw via paintGL()
    this->update();
}

void GLWidget::drawHealth(QPainter& painter, int totalHealthPlayer, int totalHealthEnemy)
{
    // Draw amount of spaceships
    float size = this->width() / 30.0f;
    float gap = 0.1;
    int height = (int) (size - 2 * size * gap);
    int width = (int) (height * 1.1);

    int spaceshipsPlayer = totalHealthPlayer / 10 + (totalHealthPlayer % 10 != 0 ? 1 : 0);
    int spaceshipsEnemy = totalHealthEnemy / 10 + (totalHealthEnemy % 10 != 0 ? 1 : 0);

    painter.setPen(Qt::green);
    painter.drawRect(QRect((int) (size * 12 + size * gap), (int) (size * gap), width, height));
    painter.drawText(QRect((int) (size * 12 + size * gap), (int) (size * gap), width, height), Qt::AlignCenter, QString::number(spaceshipsPlayer));
    painter.setPen(Qt::red);
    painter.drawRect(QRect((int) (this->width() - size - (size * 12 + size * gap)), (int) (size * gap), width, height));
    painter.drawText(QRect((int) (this->width() - size - (size * 12 + size * gap)), (int) (size * gap), width, height), Qt::AlignCenter, QString::number(spaceshipsEnemy));

    // Draw hearts
    QPixmap playerHeart("../models/player_heart.png");
    QPixmap enemyHeart("../models/enemy_heart.png");
    QPixmap emptyHeart("../models/empty_heart.png");

    int healthPlayer = totalHealthPlayer % 10 + (totalHealthPlayer % 10 == 0  && totalHealthPlayer != 0 ? 10 : 0);
    int healthEnemy = totalHealthEnemy % 10 + (totalHealthEnemy % 10 == 0 && totalHealthEnemy != 0 ? 10 : 0);

    for (int i = 0; i < 10; i++)
    {
        painter.drawPixmap((int) (size * i + size * gap), (int) (size * gap), width, height, (i < healthPlayer) ? m_playerHeart : m_emptyHeart);
        painter.drawPixmap((int) (this->width() - size - (size * i + size * gap)), (int) (size * gap), width, height, (i < healthEnemy) ? m_enemyHeart : m_emptyHeart);
    }
}

void GLWidget::drawMinimap(QPainter& painter, Hittable::Ptr player, Hittable::Ptr enemy)
{
    QPixmap minimap("../models/minimap");
    QPixmap heightMinimap("../models/height_minimap");
    QPixmap playerMinimap("../models/player_minimap");
    QPixmap enemyMinimap("../models/enemy_minimap");

    float size = this->width() * 0.2F;
    float originX = this->height() - size * 1.1F + size / 2;
    float originY = this->width() - size * 1.1F + size / 2;
    painter.translate(originY, originX);
    painter.save();
    painter.drawPixmap((int) (-size / 2), (int) (-size / 2), (int) size, (int) size, minimap);

    int fighterHeight = (int) (this->width() * 0.025);
    int fighterWidth = fighterHeight / 2;

    painter.translate(size / 10000 * player->getPosition()[0], -size / 10000 * player->getPosition()[1]);
    painter.rotate(std::atan2(player->getXAxis()[0], player->getXAxis()[1]) * 180 / M_PI);
    painter.drawPixmap(-fighterWidth / 2, -fighterHeight / 2, fighterWidth, fighterHeight, playerMinimap);

    painter.restore();
    painter.save();

    painter.translate(size / 10000 * enemy->getPosition()[0], -size / 10000 * enemy->getPosition()[1]);
    painter.rotate(std::atan2(enemy->getXAxis()[0], enemy->getXAxis()[1]) * 180 / M_PI);
    painter.drawPixmap(-fighterWidth / 2, -fighterHeight / 2, fighterWidth, fighterHeight, enemyMinimap);

    // Height display
    painter.restore();
    float heightMinimapWidth = size * 0.1;
    painter.translate(-size / 2 - heightMinimapWidth / 2, -size / 2);
    painter.drawPixmap(-heightMinimapWidth, 0, (int) heightMinimapWidth, (int) size, heightMinimap);
    painter.rotate(90);
    painter.save();
    painter.translate(size / 2 - size / 10000 * player->getPosition()[2], 0);
    painter.drawPixmap((int) (-heightMinimapWidth / 4), 0, (int) (heightMinimapWidth / 2), (int) heightMinimapWidth, playerMinimap);
    painter.restore();
    painter.translate(size / 2 - size / 10000 * enemy->getPosition()[2], 0);
    painter.drawPixmap((int) (-heightMinimapWidth / 4), 0, (int) (heightMinimapWidth / 2), (int) heightMinimapWidth, enemyMinimap);

    painter.resetTransform();
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
