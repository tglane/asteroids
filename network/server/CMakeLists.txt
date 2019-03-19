cmake_minimum_required(VERSION 3.0.2)
project(CPP18_ASTEROIDS)

find_package(Qt5Network REQUIRED)

set(SERVER_SRC
    server.cpp)

include_directories(${Qt5Network_INCLUDE_DIRS})

add_executable(server ${SERVER_SRC})
target_link_libraries(server Qt5::Network)
