#include "Controller.hpp"
#include "physics/Transformable.hpp"

namespace asteroids
{

Controller::Controller() : m_cooldown_player(0), m_cooldown_enemy(0) {}

void Controller::keyControl(std::map<Qt::Key, bool> &keyStates, Hittable::Ptr& player, Hittable::Ptr& enemy, PhysicsEngine::Ptr& physicsEngine)
{
    enemy->move(Transformable::FORWARD, 5);
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
    if (m_cooldown_enemy == 0 && keyStates[Qt::Key_M])
    {
        Bullet::Ptr bullet = make_shared<Bullet>(Bullet(enemy->getPosition() - enemy->getZAxis() * 42,
                                                        enemy->getXAxis(), enemy->getId()));
        physicsEngine->addBullet(bullet);
        m_cooldown_enemy = 10;
    }
    if (m_cooldown_enemy > 0)
    {
        m_cooldown_enemy--;
    }

    player->move(Transformable::FORWARD, 5);
    if (keyStates[Qt::Key_D])
    {
        player->rotate(Transformable::ROLL_CLOCKWISE, 0.05);
    }
    if (keyStates[Qt::Key_A])
    {
        player->rotate(Transformable::ROLL_COUNTERCLOCKWISE, 0.05);
    }
    if (keyStates[Qt::Key_W])
    {
        player->rotate(Transformable::PITCH_DOWN, 0.05);
    }
    if (keyStates[Qt::Key_S])
    {
        player->rotate(Transformable::PITCH_UP, 0.05);
    }
    if (keyStates[Qt::Key_Q])
    {
        player->rotate(Transformable::YAW_COUNTERCLOCKWISE, 0.05);
    }
    if (keyStates[Qt::Key_E])
    {
        player->rotate(Transformable::YAW_CLOCKWISE, 0.05);
    }
    if (m_cooldown_player == 0 && keyStates[Qt::Key_Space])
    {
        Bullet::Ptr bullet = make_shared<Bullet>(Bullet(player->getPosition() - player->getZAxis() * 42,
                                                        player->getXAxis(), player->getId()));
        physicsEngine->addBullet(bullet);
        m_cooldown_player = 10;
    }
    if (m_cooldown_player > 0)
    {
        m_cooldown_player--;
    }

    if (keyStates[Qt::Key_T])
    {
        player->move(Transformable::FORWARD, 5);
    }
    if (keyStates[Qt::Key_G])
    {
        player->move(Transformable::BACKWARD, 5);
    }
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
}

}
