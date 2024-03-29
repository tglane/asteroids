/**
 * Controller.cpp
 * @author Steffen Hinderink
 * @author Juri Vana
 */

#include <QtCore/QFileInfo>
#include "Controller.hpp"
#include "physics/Transformable.hpp"

namespace asteroids
{

// Setting all constants
const vector<Qt::Key> Controller::mapToQt = {Qt::Key_E, Qt::Key_Q, Qt::Key_S, Qt::Key_W, Qt::Key_A, Qt::Key_D};
const vector<Transformable::RotationTransform> Controller::mapToRotation = {Transformable::ROLL_CLOCKWISE,
                                                                         Transformable::ROLL_COUNTERCLOCKWISE,
                                                                         Transformable::PITCH_DOWN,
                                                                         Transformable::PITCH_UP,
                                                                         Transformable::YAW_COUNTERCLOCKWISE,
                                                                         Transformable::YAW_CLOCKWISE};
const int Controller::framesToMaxSpeed = 30;
const float Controller::minSpeed = 100;
const float Controller::maxSpeed = 1000;
const int Controller::framesToMaxRot = 30;
const float Controller::maxRot = 0.05;

Controller::Controller() : m_bulletCooldown(0), m_missileCooldown(0), m_gamepadR1(0), m_gamepadL1(0)
{
    m_keys = std::vector<int>(7);
    for (int i = 0; i < 7; i++)
    {
        m_keys[i] = 0;
    }
    m_gamepadAvailable = m_gamepad.init();

    m_bulletSound.setSource(QUrl::fromLocalFile(QFileInfo("../models/shooting.wav").absoluteFilePath()));
    m_bulletSound.setVolume(0.4f);
}

bool Controller::gamepadAvailable()
{
    m_gamepadAvailable = m_gamepad.init();
    return m_gamepadAvailable;
}

void Controller::keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player,
                            PhysicsEngine::Ptr& physicsEngine, udpclient& client, int elapsed_time)
{
    if (player->getHealth() > 0)
    {
        // In order to make the movements smoother the speed increases and decreases linearly over time when pressing
        // or letting go of a key.

        // Move
        if (keyStates[Qt::Key_Shift])
        {
            if (m_keys[6] < framesToMaxSpeed)
            {
                m_keys[6]++;
            }
        }
        else
        {
            if (m_keys[6] > 0)
            {
                m_keys[6]--;
            }
        }
        float speed = minSpeed + (maxSpeed - minSpeed) * ((float) m_keys[6] / framesToMaxSpeed);
        player->move(Transformable::FORWARD, speed * elapsed_time / 1000.0);

        // Rotate
        for (int i = 0; i < 6; i++)
        {
            if (keyStates[mapToQt[i]])
            {
                if (m_keys[i] < framesToMaxRot)
                {
                    m_keys[i]++;
                }
            }
            else
            {
                if (m_keys[i] > 0)
                {
                    m_keys[i]--;
                }
            }
            float rot = maxRot * ((float) m_keys[i] / framesToMaxRot);
            player->rotate(mapToRotation[i], rot);
        }

        // Shoot
        if (m_bulletCooldown > 0)
        {
            m_bulletCooldown -= elapsed_time;
            if (m_bulletCooldown < 0)
                m_bulletCooldown = 0;
        }
        if (m_bulletCooldown == 0 && keyStates[Qt::Key_Space])
        {
            m_bulletSound.play();
            client.send_bullet(player->getPosition(), player->getXAxis(), player->getZAxis());
            m_bulletCooldown = 200;
        }

        if (m_missileCooldown > 0)
        {
            m_missileCooldown -= elapsed_time;
            if (m_missileCooldown < 0)
                m_missileCooldown = 0;
        }
        if (m_missileCooldown == 0 && keyStates[Qt::Key_R])
        {
            m_bulletSound.play();
            client.send_missile(player->getPosition(), player->getXAxis(), player->getYAxis(), player->getZAxis());
            m_missileCooldown = 1000;
        }
    }
}

void Controller::gamepadControl(Hittable::Ptr& player, PhysicsEngine::Ptr& physicsEngine, udpclient& client, int elapsed_time)
{
    if (m_gamepadAvailable)
    {
        if (player->getHealth() > 0)
        {
            // Move
            float speed = minSpeed + (maxSpeed - minSpeed) * (float) m_gamepad.getR2();
            player->move(Transformable::FORWARD, speed * elapsed_time / 1000.0);

            // Rotate
            float rot = maxRot * (float) m_gamepad.getLeftX();
            player->rotate(Transformable::YAW_CLOCKWISE, rot);
            rot = maxRot * (float) m_gamepad.getLeftY();
            player->rotate(Transformable::PITCH_DOWN, rot);
            if (m_gamepad.isR1Pressed())
            {
                if (m_gamepadR1 < framesToMaxRot)
                {
                    m_gamepadR1++;
                }
            }
            else
            {
                if (m_gamepadR1 > 0)
                {
                    m_gamepadR1--;
                }
            }
            rot = maxRot * ((float) m_gamepadR1 / framesToMaxRot);
            player->rotate(Transformable::ROLL_CLOCKWISE, rot);
            if (m_gamepad.isL1Pressed())
            {
                if (m_gamepadL1 < framesToMaxRot)
                {
                    m_gamepadL1++;
                }
            }
            else
            {
                if (m_gamepadL1 > 0)
                {
                    m_gamepadL1--;
                }
            }
            rot = maxRot * ((float) m_gamepadL1 / framesToMaxRot);
            player->rotate(Transformable::ROLL_COUNTERCLOCKWISE, rot);

            // Shoot
            if (m_bulletCooldown > 0)
            {
                m_bulletCooldown -= elapsed_time;
                if (m_bulletCooldown < 0)
                    m_bulletCooldown = 0;
            }
            if (m_bulletCooldown == 0 && m_gamepad.isAPressed())
            {
                Bullet::Ptr bullet = make_shared<Bullet>(Bullet(player->getPosition() - player->getZAxis() * 42,
                                                                player->getXAxis(), player->getId()));
                m_bulletSound.play();
                client.send_bullet(player->getPosition(), player->getXAxis(), player->getZAxis());
                physicsEngine->addBullet(bullet);
                m_bulletCooldown = 200;
            }

            if (m_missileCooldown > 0)
            {
                m_missileCooldown -= elapsed_time;
                if (m_missileCooldown < 0)
                    m_missileCooldown = 0;
            }
            if (m_missileCooldown == 0 && m_gamepad.isBPressed())
            {
                m_bulletSound.play();
                client.send_missile(player->getPosition(), player->getXAxis(), player->getYAxis(), player->getZAxis());
                m_missileCooldown = 1000;
            }
        }
    }
    else
    {
        std::cout << "gamepad unavailable" << std::endl;
    }
}

}
