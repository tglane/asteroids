
#ifndef CPP18_ASTEROIDS_UDPCLIENT_HPP
#define CPP18_ASTEROIDS_UDPCLIENT_HPP

#include <QObject>
#include <QUdpSocket>
#include <memory>

#include "math/Vector.hpp"
#include "math/Quaternion.hpp"
//#include "physics/PhysicsEngine.hpp"
#include "rendering/SpaceCraft.hpp"

class udpclient: public QObject {
    Q_OBJECT

public:

    explicit udpclient(QObject *parent = 0);

    /**
     * @brief Sends own position, flight direction and rotation to the server
     */
    void send_position(asteroids::Vector<float> position, asteroids::Vector<float> velocity, asteroids::Vector<float> xAxis, asteroids::Vector<float> yAxis, asteroids::Vector<float> zAxis);

    /**
     * @brief Sends position and flight direction of a new bullet to the server
     */
    void send_bullet();

    //void setPhysicsPtr(asteroids::PhysicsEngine::Ptr phyEng) { m_physicsEngine = std::move(phyEng); }

    void setOtherFighter(asteroids::SpaceCraft::Ptr other_fighter) { m_otherFighter = std::move(other_fighter); }

signals:

public slots:

    /**
     * @brief
     */
    void readyRead();

private:

    void setPosFromPackage(int id, char* data);

    QUdpSocket *socket;

    int m_id;

    unsigned int seq_number;

    //asteroids::PhysicsEngine::Ptr m_physicsEngine;

    asteroids::SpaceCraft::Ptr m_otherFighter;
};

#endif //CPP18_ASTEROIDS_UDPCLIENT_HPP
