/*
 * ReadPPM.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "ReadPPM.hpp"

namespace asteroids
{

ReadPPM::ReadPPM(string filename)
{
    // Try to open file
    ifstream in(filename.c_str());


    // Parse file
    if(in.good())
    {
        // Line buffer
        char buffer[1024];

        // Read file tag
        readLine(in, buffer);

        // Check tag
        string tag(buffer);
        if(tag != "P3")
        {
            cout << "ReadPPM: Format " << tag << " is unsupported" << endl;
            return;
        }

        // Read width, height and color information
        stringstream ss;
        readLine(in, buffer);
        ss << buffer << " ";
        readLine(in, buffer);
        ss << buffer << " ";

        // Read formatted data
        ss >> m_width >> m_height;

        // Alloc data
        m_pixels = Ptr(new unsigned char[m_width * m_height * 3]);

        // Read pixels
        int p;
        for(int i = 0; i < m_width * m_height * 3; i++)
        {
            in >> p;
            m_pixels[i] = (unsigned char)p;
        }
    }
    else
    {
        cout << "ReadPPM: Unable to open file " << filename << "." << endl;
    }
}

void ReadPPM::readLine(ifstream & in, char* buffer)
{
    //Read lines until no comment line
    //was found
    do
    {
      in.getline(buffer, 256);
    }
    while(buffer[0] == '#' && in.good() );
}

}
