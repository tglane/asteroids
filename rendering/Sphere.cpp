/*
 *  Sphere.cpp
 *
 *  Created on: Nov. 25 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Sphere.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace asteroids
{

Sphere::Sphere(
    const Vector3f& position, float radius, 
    int numSides, int numStacks)
{
    m_radius = radius;
    m_numSides = numSides;
    m_numStacks = numStacks;
    m_position = position;
   
}

int Sphere::radius()
{
    return m_radius;
}
    
void Sphere::render()
{
    
     // Compute transformation matrix
	computeMatrix();

    glColor3f(1.0, 1.0, 1.0);

	// Push old transformation of the OpenGL matrix stack and
	// start rendering the mesh in according to the
	// internal transformation matrix
	// glPushMatrix();
	// glMultMatrixf(m_transformation.getData());

    float curRadius, curTheta, curRho, deltaTheta, deltaRho, curX,curY,curZ;
    int curStack, curSlice, numVerts = (m_numStacks-1)*m_numSides;
    Vector3f points[numVerts];
    int curVert = 0;
    int t;

    deltaTheta = (2*M_PI) / m_numSides;
    deltaRho = M_PI / m_numStacks;

        for (curStack=1; curStack< m_numStacks; curStack++)
        {
            curRho = (3.141/2.0) - curStack*deltaRho;
            curY = sin(curRho) * m_radius;
            curRadius = cos(curRho) * m_radius;
            for (curSlice=0; curSlice< m_numSides; curSlice++)
            {
                curTheta = curSlice * deltaTheta;
                curX = curRadius * cos(curTheta);
                curZ = -curRadius * sin(curTheta);
                points[curVert++] = Vector3f(curX,curY,curZ);
            }
        }

        glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,1,0);
        glVertex3d(0, m_radius,0);
        for (t=0; t< m_numSides; t++)
        {
            curX = points[t][0];
            curY = points[t][1];
            curZ = points[t][2];
            glNormal3d(curX, curY, curZ);
            glVertex3d(curX, curY, curZ);
        }
            curX = points[0][0];
            curY = points[0][1];
            curZ = points[0][2];
        glNormal3d(curX, curY, curZ);
        glVertex3d(curX, curY, curZ);
        glEnd();

        int vertIndex;
    for (curStack=0; curStack< m_numStacks-2; curStack++)
    {
        vertIndex = curStack * m_numSides;
        glBegin(GL_QUAD_STRIP);
            for (curSlice=0; curSlice< m_numSides; curSlice++)
            {
                glNormal3d(points[vertIndex+curSlice][0], points[vertIndex+curSlice][1], points[vertIndex+curSlice][2]);
                glVertex3d(points[vertIndex+curSlice][0], points[vertIndex+curSlice][1], points[vertIndex+curSlice][2]);

                glNormal3d(points[vertIndex+ m_numSides + curSlice][0], points[vertIndex+m_numSides+curSlice][1], points[vertIndex+m_numSides+curSlice][2]);
                glVertex3d(points[vertIndex+ m_numSides + curSlice][0], points[vertIndex+m_numSides+curSlice][1], points[vertIndex+m_numSides+curSlice][2]);
            }
            glNormal3d(points[vertIndex][0], points[vertIndex][1], points[vertIndex][2]);
            glVertex3d(points[vertIndex][0], points[vertIndex][1], points[vertIndex][2]);
            glNormal3d(points[vertIndex+ m_numSides][0], points[vertIndex+m_numSides][1], points[vertIndex+m_numSides][2]);
            glVertex3d(points[vertIndex+ m_numSides][0], points[vertIndex+m_numSides][1], points[vertIndex+m_numSides][2]);
        glEnd();
    }

    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,-1,0);
        glVertex3d(0,- m_radius,0);
        for (t=0; t< m_numSides-1; t++)
        {
            curX = points[numVerts-1-t][0];
            curY = points[numVerts-1-t][1];
            curZ = points[numVerts-1-t][2];
            glNormal3d(curX, curY, curZ);
            glVertex3d(curX, curY, curZ);
        }
            curX = points[numVerts-1][0];
            curY = points[numVerts-1][1];
            curZ = points[numVerts-1][2];
        glNormal3d(curX, curY, curZ);
        glVertex3d(curX, curY, curZ);
    glEnd();
    //glPopMatrix();
}

}