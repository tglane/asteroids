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
}

void Controller::keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player, Hittable::Ptr& enemy,
                            PhysicsEngine::Ptr& physicsEngine, int elapsed_time)
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
            Bullet::Ptr bullet = make_shared<Bullet>(Bullet(player->getPosition() - player->getZAxis() * 42,
                                                            player->getXAxis(), player->getId()));
            physicsEngine->addBullet(bullet);
            m_cooldownPlayer = 300;
        }

        /*
        if (keyStates[Qt::Key_F])
        {
            player->move(Transformable::STRAFE_LEFT, 5);
        }
        if (keyStates[Qt::Key_H])
        {
            player->move(Transformable::STRAFE_RIGHT, 5);
        }
        if (keyStates[Qt::Key_R])
        {
            player->move(Transformable::LIFT_UP, 5);
        }
        if (keyStates[Qt::Key_Z])
        {
            player->move(Transformable::LIFT_DOWN, 5);
        }
        */
    }

    /*
    if (enemy->getHealth() > 0)
    {
        enemy->move(Transformable::FORWARD, 300 * elapsed_time / 1000.0);
        if (keyStates[Qt::Key_L])
        {
            enemy->rotate(Transformable::ROLL_CLOCKWISE, 0.05);
        }
        if (keyStates[Qt::Key_J])
        {
            enemy->rotate(Transformable::ROLL_COUNTERCLOCKWISE, 0.05);
        }
        if (keyStates[Qt::Key_I])
        {
            enemy->rotate(Transformable::PITCH_DOWN, 0.05);
        }
        if (keyStates[Qt::Key_K])
        {
            enemy->rotate(Transformable::PITCH_UP, 0.05);
        }
        if (keyStates[Qt::Key_U])
        {
            enemy->rotate(Transformable::YAW_COUNTERCLOCKWISE, 0.05);
        }
        if (keyStates[Qt::Key_O])
        {
            enemy->rotate(Transformable::YAW_CLOCKWISE, 0.05);
        }
        // Add a bullet to physics engine
        if (m_cooldownEnemy > 0)
        {
            m_cooldownEnemy -= elapsed_time;
            if (m_cooldownEnemy < 0)
                m_cooldownEnemy = 0;
        }
        if (m_cooldownEnemy == 0 && keyStates[Qt::Key_M])
        {
            Bullet::Ptr bullet = make_shared<Bullet>(Bullet(enemy->getPosition() - enemy->getZAxis() * 42,
                                                            enemy->getXAxis(), enemy->getId()));
            physicsEngine->addBullet(bullet);
            m_cooldownEnemy = 300;
        }
    }
    */
}

}
