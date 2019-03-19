cmake_minimum_required(VERSION 3.0.2)
project(CPP18_ASTEROIDS)

find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)


set(SERVER_SRC
    server.cpp)

include_directories(../light_classes/)
include_directories(../../light_classes/)
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_cpp(SERVER_HDR_MOC server.h)

add_executable(server ${SERVER_SRC} ${SERVER_HDR_MOC})

target_link_libraries(server Qt5::Network)
target_link_libraries(server Qt5::Widgets)
