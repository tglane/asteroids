/*
 *  Camera.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "view/Camera.hpp"

namespace asteroids
{

Camera::Camera() = default;

void Camera::apply()
{
    /* Clear matrix stack */
    glLoadIdentity();

    float eyeX = getPosition()[0];
    float eyeY = getPosition()[1];
    float eyeZ = getPosition()[2];
    float centerX = eyeX + getXAxis()[0];
    float centerY = eyeY + getXAxis()[1];
    float centerZ = eyeZ + getXAxis()[2];
    float upX = getZAxis()[0];
    float upY = getZAxis()[1];
    float upZ = getZAxis()[2];
    /*
    std::cout << eyeX << " " << eyeY << " " << eyeZ << " | "
              << centerX << " " << centerY << " " << centerZ << " | "
              << upX << " " << upY << " " << upZ << std::endl;
    */
    /* Apply transformation */
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

}
