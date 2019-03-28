cmake_minimum_required(VERSION 3.0.2)
project(CPP18_ASTEROIDS)

find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

if(OpenGL_FOUND)
    link_directories(${OpenGL_LIBRARY_DIRS})
    include_directories(${OpenGL_INCLUDE_DIRS})
endif(OpenGL_FOUND)

if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
endif(GLEW_FOUND)

set(SERVER_SRC
    TcpServer.cpp
    UdpServer.cpp
    ServerPhysicsEngine.cpp
    TcpServer.cpp
    Server.cpp
    ../UdpPeer.cpp
    ../../math/Quaternion.cpp
    ../../math/Matrix.cpp
    ../../physics/Transformable.cpp
    ../../physics/PhysicalObject.cpp
    ../../physics/PhysicalBullet.cpp
    ../../physics/BasePhysicsEngine.cpp
    ../../physics/Hittable.cpp
    ../../datamodel/DataModel_Server.cpp
    ../../datamodel/Planet.cpp
    ../../datamodel/Player.cpp
    ../../datamodel/ShipOrder.cpp
    ../../datamodel/MoveOrder.cpp
    ../../datamodel/Battle.cpp
    ../../datamodel/MineOrder.cpp
    ../../util/AsteroidField.cpp
    ../../math/Randomizer.cpp)

set(SERVER_HDR
    UdpServer.hpp
    TcpServer.hpp
    ../UdpPeer.hpp)

set(CMAKE_AUTOMOC ON)

include_directories(../light_classes/)
include_directories(../../)
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

#qt5_wrap_cpp(SERVER_HDR_MOC ${SERVER_HDR})

add_executable(server ${SERVER_SRC} ${SERVER_HDR_MOC})

target_link_libraries(server Qt5::Network)
target_link_libraries(server Qt5::Widgets)
