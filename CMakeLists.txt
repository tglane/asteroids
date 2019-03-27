###############################################################################
# Declare the minimum cmake version required
###############################################################################
cmake_minimum_required(VERSION 3.0.2)

###############################################################################
# The name of out project
###############################################################################
project(CPP18_ASTEROIDS)

###############################################################################
# Now you would normally declare additional linker and include directories.
# We don't need this at this moment, just to show you how its done
###############################################################################

#link_directories("${CMAKE_SOURCE_DIR}/lib")
#link_directories("$ENV{HOME}/local/lib")

#include_directories(${CMAKE_SOURCE_DIR}/ext/Eigen)
#include_directories(${CMAKE_SOURCE_DIR})
#include_directories(${CMAKE_SOURCE_DIR}/src/lib)

###############################################################################
# Extend path to search for local cmake modules
###############################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

###############################################################################
# With cmake we can define additional compiler flags for different 
# configurations. CMAKE_CXX_FLAGS are for the default case. The debug config
# can be used to generate debug symbols for gdb. The release option uses
# special optimization flags
###############################################################################


set( CMAKE_CXX_FLAGS         "-O3 -Wall -std=c++14 -lstdc++fs" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -msse3 -Wno-deprecated-declarations -Wno-unused -Wcpp -lstdc++fs" )
set( CMAKE_CXX_FLAGS_DEBUG   "-g -Wall -lstdc++fs" )

#set( CMAKE_EXE_LINKER_FLAGS  "-L/usr/local/lib -lSDL -lSDLmain -lSDL")

###############################################################################
# Find required libraries.Right now we need glut and OpenGL. The required 
# options forces this packages to be present. For non-mandatory packages you 
# can leave this flag out. The can then check if they were found by using
# IF(OpenGL_found) etc.
###############################################################################

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/ext/load3ds)
include_directories(${CMAKE_SOURCE_DIR}/ext/load3ds/math3d++)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED) 
find_package(JPEG REQUIRED)

# Add SDL2 include directory to search path
include_directories(${SDL2_INCLUDE_DIR})

if(OpenGL_FOUND)
    link_directories(${OpenGL_LIBRARY_DIRS})
    include_directories(${OpenGL_INCLUDE_DIRS})
endif(OpenGL_FOUND)

if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
endif(GLEW_FOUND)

set(Boost_USE_MULTITHREADED     ON)

find_package( Boost 1.54
    COMPONENTS
    program_options
    filesystem
    system
    thread
    serialization
    REQUIRED )

link_directories(${Boost_LIBRARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

###############################################################################
# Find and setup Qt5 
###############################################################################

set( QT_USE_QTOPENGL TRUE )
find_package( Qt5
    COMPONENTS
    Core
    Widgets
    OpenGL
    Gamepad
    Multimedia
    Network
    REQUIRED
)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})
include_directories(${Qt5Multimedia_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})

###############################################################################
# Compile external libraries
###############################################################################

add_subdirectory(ext/load3ds)

###############################################################################
# Variable for the sources of the binary. In larger projects it is often a 
# good idea to use such variables because it is easy to add more source files
# and you can reuse it for several targets (maybe you want to build a library
# some day ;-)
###############################################################################

set(ASTEROID_SOURCES
    Main.cpp
    #io/PLYIO.cpp #old ply loader
    io/PLYTraits.cpp
    io/ReadPPM.cpp
    io/ReadTGA.cpp
    io/ReadJPG.cpp
    io/ReadPLY.cpp
    io/Read3DS.cpp
    io/ReadOBJ.cpp
    io/TextureFactory.cpp
    io/TriangleMeshFactory.cpp
    io/LevelParser.cpp
    math/Matrix.cpp
    math/Quaternion.cpp
    math/Randomizer.cpp
    physics/Transformable.cpp
    physics/PhysicalObject.cpp
    physics/PhysicalBullet.cpp
    physics/PhysicsEngine.cpp
    physics/BasePhysicsEngine.cpp
    physics/ParticleEngine.cpp
    physics/Hittable.cpp
    physics/PhysicalBullet.cpp
    rendering/Bullet.cpp
    rendering/ParticleEffect.cpp
    rendering/Sphere.cpp
    rendering/Texture.cpp
    rendering/TriangleMesh.cpp
    rendering/TexturedMesh.cpp
    rendering/Skybox.cpp
    rendering/SpaceCraft.cpp
    rendering/Asteroid.cpp
    util/AsteroidField.cpp 
    util/ColorMap.cpp   
    util/Util.cpp
    view/GLWidget.cpp
    view/MainWindow.cpp
    network/client/udpclient.cpp
    network/client/tcpclient.cpp
    datamodel/DataModel.cpp
    datamodel/DataModel_Server.cpp
    datamodel/Planet.cpp
    datamodel/Player.cpp
    datamodel/MineOrder.cpp
    datamodel/ShipOrder.cpp
    datamodel/MoveOrder.cpp
    datamodel/Battle.cpp
    view2D/StartingDialog.cpp
    view2D/MainWindow2D.cpp
    view2D/MyEllipse.cpp
    view2D/EndWindow.cpp
    view2D/GameWindow.cpp
    view2D/SwitchingWindowInfo.cpp
    view/Camera.cpp
    view/Controller.cpp
    Gamepad.cpp
)

set(ASTEROID_MOCS
    view/MainWindow.hpp
    view2D/MainWindow2D.hpp
    view2D/StartingDialog.hpp
    view2D/EndWindow.hpp
    view2D/GameWindow.hpp
    view2D/SwitchingWindowInfo.hpp
    datamodel/DataModel.hpp
    datamodel/DataModel_Server.hpp
)

set(ASTEROID_UIS
    view/MainWindow.ui
    view2D/MainWindow2D.ui
    view2D/StartingDialog.ui
    view2D/EndWindow.ui
    view2D/GameWindow.ui
    view2D/SwitchingWindowInfo.ui
)

set(CMAKE_AUTOMOC ON)

qt5_wrap_ui(ASTEROID_UI_HDRS ${ASTEROID_UIS})
message(STATUS ${ASTEROID_UI_HDRS})


add_definitions(${QT_DEFINITIONS})


set(ASTEROID_DEPENDS     
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${GLEW_LIBRARIES} 
    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARY} 
    ${SDL2_LIBRARY}
    ${JPEG_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Multimedia_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    c3dsreader
)

###############################################################################
# The executable of our project
###############################################################################

add_executable(asteroids
    ${ASTEROID_SOURCES}
    ${C3DSREADER_SOURCES}
    ${ASTEROID_MOCS}
    ${ASTEROID_UI_HDRS}
    ${STRATEGY_MOCS}
    ${STRATEGY_UI_HDRS}
    ${START_MOCS}
    ${START_UI_HDRS}
)
###############################################################################
# External library dependencys
###############################################################################

#target_link_libraries(asteroids SOIL)
target_link_libraries(asteroids Qt5::Gamepad ${ASTEROID_DEPENDS} /usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a)
