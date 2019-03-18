/*
 *  TextureMesh.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "TexturedMesh.hpp"

namespace asteroids
{

TexturedMesh::TexturedMesh() : TriangleMesh()
{

}

TexturedMesh::TexturedMesh( TexturedMesh &mesh) : TriangleMesh(mesh)
{
	vector<Material*>::iterator mat_it;
    for(size_t i = 0; i < mesh.m_materials.size(); i++)
    {
    	Material::Ptr old_mat = mesh.m_materials[i];
    	Material::Ptr new_mat(new Material);
    	new_mat->m_ambient = old_mat->m_ambient;
    	new_mat->m_diffuse = old_mat->m_diffuse;
    	new_mat->m_specular = old_mat->m_specular;
    	new_mat->m_shininess = old_mat->m_shininess;
    	new_mat->m_texture = Texture::Ptr(old_mat->m_texture);

    	m_materials.push_back(new_mat);
    }

    for(MaterialFaceLists::iterator it = mesh.m_matFaceLists.begin(); it != mesh.m_matFaceLists.end(); it++)
    {
    	MaterialFaceList::Ptr new_matList (new MaterialFaceList);
    	MaterialFaceList::Ptr old_matList = *it;

    	new_matList->m_matIndex = old_matList->m_matIndex;
    	new_matList->m_faces = old_matList->m_faces;
    	m_matFaceLists.push_back(new_matList);
    }

    m_textureCoords = FloatArray(new float [2 * m_numVertices]);
    for(int i = 0; i < 2 * m_numVertices; i++)
    {
    	m_textureCoords[i] = mesh.m_textureCoords[i];
    }

}

void TexturedMesh::render()
{
    MaterialFaceLists::iterator matListIt;
    list<int>::iterator matFaceIt;

    glPushMatrix();
	
    for(matListIt = m_matFaceLists.begin(); matListIt != m_matFaceLists.end(); matListIt++)
    {
        // Get list for current material
        MaterialFaceList::Ptr matList = *matListIt;

        // Get material
        Material::Ptr mat = m_materials[matList->m_matIndex];

        // Bind texture and set material properties
        if(mat->m_texture != 0)
        {
            mat->m_texture->bind();
        }
        setColorMaterial(mat->m_ambient, mat->m_diffuse, mat->m_specular, mat->m_shininess);

        // Render face group
        matFaceIt = matList->m_faces.begin();
        while(matFaceIt != matList->m_faces.end())
        {
            int a = *matFaceIt;
            ++matFaceIt;
            int b = *matFaceIt;
            ++matFaceIt;
            int c = *matFaceIt;
            ++matFaceIt;

            int a_pos = a * 3;
            int b_pos = b * 3;
            int c_pos = c * 3;
            
            int ta = a * 2;
            int tb = b * 2;
            int tc = c * 2;

            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_TRIANGLES);
            glTexCoord2f(m_textureCoords[ta], m_textureCoords[ta + 1]);
            glNormal3f(m_normalBuffer[a_pos], m_normalBuffer[a_pos + 1], m_normalBuffer[a_pos + 2]);
            glVertex3f(m_vertexBuffer[a_pos], m_vertexBuffer[a_pos + 1], m_vertexBuffer[a_pos + 2]);

            glTexCoord2f(m_textureCoords[tb], m_textureCoords[tb + 1]);
            glNormal3f(m_normalBuffer[b_pos], m_normalBuffer[b_pos + 1], m_normalBuffer[b_pos + 2]);
            glVertex3f(m_vertexBuffer[b_pos], m_vertexBuffer[b_pos + 1], m_vertexBuffer[b_pos + 2]);

            glTexCoord2f(m_textureCoords[tc], m_textureCoords[tc + 1]);
            glNormal3f(m_normalBuffer[c_pos], m_normalBuffer[c_pos + 1], m_normalBuffer[c_pos + 2]);
            glVertex3f(m_vertexBuffer[c_pos], m_vertexBuffer[c_pos + 1], m_vertexBuffer[c_pos + 2]);
            glEnd();
        }
    }
    glPopMatrix();

}

void TexturedMesh::setColorMaterial(Color &ambient, Color &diffuse, Color &specular, float shininess)
{
    GLfloat mat_specular[4] = {specular.r, specular.g, specular.b, 1.0};
    GLfloat mat_ambient[4]  = {ambient.r, ambient.g, ambient.b, 1.0};
    GLfloat mat_diffuse[4]  = {diffuse.r, diffuse.g, diffuse.b, 1.0};
    GLfloat mat_shininess[]  = {shininess};

    // cout << specular.r << " " << specular.g << " " << specular.b << endl;
    // cout << ambient.r << " " << ambient.g << " " << ambient.b << endl;
    // cout << diffuse.r << " " << diffuse.g << " " << diffuse.b << endl << endl;
 
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

}

}
