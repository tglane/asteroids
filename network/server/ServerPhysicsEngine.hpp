#ifndef SERVER_PHYSICS_ENGINE
#define SERVER_PHYSICS_ENGINE

#include <physics/BasePhysicsEngine.hpp>

class ServerPhysicsEngine: public BasePhysicsEngine
{
private:
    UdpServer &server;
public:
    ServerPhysicsEngine(UdpServer &server): server(server) {}
    virtual void handle_collision(PhysicalObject::Ptr& obj1, PhysicalObject::Ptr& obj2);
}

#endif
