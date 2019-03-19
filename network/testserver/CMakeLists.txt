cmake_minimum_required(VERSION 3.0.2)
project(CPP18_ASTEROIDS)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#QT5_WRAP_UI(server configwin.ui)

find_package(Qt5Network REQUIRED)

QT5_WRAP_CPP(blub testserver_tcp.hpp)

set(SERVER_SRC
    testserver_tcp.cpp)

include_directories(${Qt5Network_INCLUDE_DIRS})

add_executable(server ${blub} ${SERVER_SRC})
target_link_libraries(server Qt5::Network)
