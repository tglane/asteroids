cmake_minimum_required(VERSION 3.0.2)
project(CPP18_ASTEROIDS)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)

set(SRC
    testclient.cpp)

include_directories(../light_classes/)
include_directories(${Qt5Network_INCLUDE_DIRS})

qt5_wrap_cpp(client_hdr_moc testclient.h)
add_executable(testclient ${SRC} ${client_hdr_mov})
target_link_libraries(testclient Qt5::Network)
