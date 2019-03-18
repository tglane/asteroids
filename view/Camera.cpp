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

Camera::Camera(const Vector3f& pos, const float& turnSpeed, const float& moveSpeed)
{
    /* Init pos */
    for(int i = 0; i < 3; i++)
    {
        m_initial[i] = pos[i];        
    }

    m_trans[0] = 0.0f;
    m_trans[1] = 0.0f;
    m_trans[2] = 0.0f;

    
    /* Init look up vector */
    m_up[0]  = 0.0f;
    m_up[1]  = 1.0f;
    m_up[2]  = 0.0f;
    
    /* Init look at vector */
    m_l[0]   = 0.0f;
    m_l[1]   = 0.0f;
    m_l[2]   = 0.0f;
    
    /* Init rotation angles */
    m_rot[0] = 0.0f;
    m_rot[1] = 0.0f;
    m_rot[2] = 0.0f;
    
    /* Set speed parameters */
    m_turnSpeed = turnSpeed;
    m_moveSpeed = moveSpeed;
}


void Camera::move(const CameraMovement& dir)
{
    switch(dir)
    {
        case FORWARD:
            m_trans[0] += m_moveSpeed * sin(m_rot[1]);
            m_trans[2] += m_moveSpeed * cos(m_rot[1]);
            break;
        case BACKWARD:
            m_trans[0] -= m_moveSpeed * sin(m_rot[1]);
            m_trans[2] -= m_moveSpeed * cos(m_rot[1]);
            break;
        case LEFT:
            m_trans[0] -= m_moveSpeed * sin(PH - m_rot[1]);
            m_trans[2] -= m_moveSpeed * cos(PH - m_rot[1]);
            break;
        case RIGHT:
            m_trans[0] += m_moveSpeed * sin(PH + m_rot[1]);
            m_trans[2] += m_moveSpeed * cos(PH + m_rot[1]);
            break;
        case UP:
            m_trans[1] += m_moveSpeed;
            break;
        case DOWN:
            m_trans[1] -= m_moveSpeed;
            break;
    }
}


void Camera::turn(const CameraMovement& dir)
{
    if(dir == UP)
    {
        if(m_rot[0] < PH)
        {
            m_rot[0] -= m_turnSpeed;
        }
    }
    else if(dir == DOWN)
    {
        if(m_rot[0] < PH)
        {
            m_rot[0] += m_turnSpeed;
        }
    }
    else if(dir == LEFT)
    {
        m_rot[1] -= m_turnSpeed;
    }

    else if(dir == RIGHT)
    {
        m_rot[1] += m_turnSpeed;
    }
}

void Camera::apply()
{
    /* Calc look at vector based on rotation state */
    m_l[0] =  m_initial[0] + m_trans[0] + sin(m_rot[1]);
    m_l[2] = -m_initial[2] - m_trans[2] - cos(m_rot[1]);
    m_l[1] =  m_initial[1] + m_trans[1] + sin(m_rot[0]);

    /* Clear matrix stack */
    glLoadIdentity();
    
    /* Apply transformation */
    gluLookAt(m_initial[0] + m_trans[0], m_initial[1] + m_trans[1], -m_initial[2] - m_trans[2],
              m_l[0], m_l[1], m_l[2],
              m_up[0], m_up[1], m_up[2]);
}

void Camera::applyRotationOnly()
{
    float lx =   sin(m_rot[1]);
    float lz = - cos(m_rot[1]);
    float ly = - sin(m_rot[0]);

    glLoadIdentity();
    gluLookAt(lx, ly, lz, 0, 0, 0, 0.0, 1.0, 0.0);
}

} // namespace asteroids
