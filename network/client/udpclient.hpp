
#ifndef CPP18_ASTEROIDS_UDPCLIENT_HPP
#define CPP18_ASTEROIDS_UDPCLIENT_HPP

#include "util/gl_includes.h"

#include <QObject>
#include <QUdpSocket>
#include <memory>
#include <QJsonObject>

#include "math/Vector.hpp"
#include "math/Quaternion.hpp"
#include "physics/PhysicsEngine.hpp"
#include "rendering/SpaceCraft.hpp"

class udpclient: public QObject {
    Q_OBJECT

public:

    using Ptr = std::shared_ptr<udpclient>;

    udpclient(int id, QString ip, int port, QObject *parent = 0);

    /**
     * @brief Sends own position, flight direction and rotation to the server
     */
    void send_position(asteroids::Vector<float> position, asteroids::Vector<float> velocity, asteroids::Vector<float> xAxis, asteroids::Vector<float> yAxis, asteroids::Vector<float> zAxis);

    /**
     * @brief Sends position and flight direction of a new bullet to the server
     */
    void send_bullet(asteroids::Vector3f position, asteroids::Vector3f xAxis, asteroids::Vector3f zAxis);

    /**
     * @brief Receives a collision package from the server
     * @param recv_seq_nr
     * @param data
     */
    void recv_collision(int recv_seq_nr, char* data);

    /**
     * @brief Receives an acknowledge package from the server to delete not acknowledged packages
     */
    void recv_ack(int recv_seq_nr, int recv_id);

    /**
     * @brief Sends all not acknowledged packages again to the server
     */
    void send_not_acknowledged();

    /**
     * @brief Sets the physics engine
     */
    void setPhysicsPtr(asteroids::PhysicsEngine::Ptr phyEng) { m_physicsEngine = std::move(phyEng); }

    /**
     * @biref Sets enemy fighter pointer
     * @param other_fighter
     */
    void setOtherFighter(asteroids::SpaceCraft::Ptr other_fighter) { m_otherFighter = std::move(other_fighter); }

    /// Get own id
    int get_id() { return m_id; }

signals:

public slots:
    /**
     * @brief read incoming data from udp server
     */
    void readyRead();

    /**
     * @brief inits the udp data connection for the 3d fight
     * @param init_data containing the position of the spaceships and asteroids
     */
    void init_fight_slot(QJsonObject init_data);

private:
    /**
     * @brief Sets the own position and enemy position from a received package from server
     * @param recv_id
     * @param data
     */
    void setPosFromPackage(int recv_id, char* data);

    /**
     * @brief Creates a bullet fired from an enemy
     * @param recv_seq_nr
     * @param recv_id
     * @param data
     */
    void createNewBulletFromPackage(int recv_seq_nr, int recv_id, char* data);

    std::shared_ptr<QUdpSocket> socket;

    int m_id;

    QString m_ip;

    unsigned int seq_number;

    asteroids::PhysicsEngine::Ptr m_physicsEngine;

    asteroids::SpaceCraft::Ptr m_otherFighter;

    /// map for not acknowledged packages
    std::map<int, QByteArray> m_not_acknowledged;
};

#endif //CPP18_ASTEROIDS_UDPCLIENT_HPP
