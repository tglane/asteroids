#include "Controller.hpp"
#include "physics/Transformable.hpp"

namespace asteroids
{

const vector<Qt::Key> Controller::mapToQt = {Qt::Key_D, Qt::Key_A, Qt::Key_W, Qt::Key_S, Qt::Key_Q, Qt::Key_E};
const vector<Transformable::RotationTransform> Controller::mapToAngle = {Transformable::ROLL_CLOCKWISE,
                                                                         Transformable::ROLL_COUNTERCLOCKWISE,
                                                                         Transformable::PITCH_DOWN,
                                                                         Transformable::PITCH_UP,
                                                                         Transformable::YAW_COUNTERCLOCKWISE,
                                                                         Transformable::YAW_CLOCKWISE};

Controller::Controller() : m_cooldownPlayer(0), m_cooldownEnemy(0)
{
    m_keys = std::vector<int>(7);
    for (int i = 0; i < 7; i++)
    {
        m_keys[i] = 0;
    }
    m_gamepadAvailable = m_gamepad.init();
}

void Controller::keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player,
                            PhysicsEngine::Ptr& physicsEngine, udpclient& client, int elapsed_time)
{
    if (player->getHealth() > 0)
    {
        // Move
        int framesToMaxSpeed = 60;
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
        float minSpeed = 100;
        float maxSpeed = 800;
        float speed = minSpeed + (maxSpeed - minSpeed) / 30 * m_keys[6];
        player->move(Transformable::FORWARD, speed * elapsed_time / 1000.0);

        // Rotate
        for (int i = 0; i < 6; i++)
        {
            int framesToMaxRot = 30;
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
            float maxRot = 0.1;
            float rot = maxRot / framesToMaxSpeed * m_keys[i];
            player->rotate(mapToAngle[i], rot);
        }

        // Shoot
        if (m_cooldownPlayer > 0)
        {
            m_cooldownPlayer -= elapsed_time;
            if (m_cooldownPlayer < 0)
                m_cooldownPlayer = 0;
        }
        if (m_cooldownPlayer == 0 && keyStates[Qt::Key_Space])
        {
            client.send_bullet(player->getPosition(), player->getXAxis(), player->getZAxis());
            m_cooldownPlayer = 300;
        }
    }
}

void Controller::gamepadControl(Hittable::Ptr& player, PhysicsEngine::Ptr& physicsEngine, udpclient& client, int elapsed_time)
{
    if (m_gamepadAvailable)
    {
        std::cout << m_gamepad.getR2() << std::endl;
    }
    else
    {
        std::cout << "Kein Gamepad gefunden" << std::endl;
    }
}

}
