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

#ifndef READPPM_HPP_
#define READPPM_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using std::string;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

#include "BitmapReader.hpp"

namespace asteroids
{

/**
 * @brief   Reads a .ppm-File in ASCII-Format (P4)
 */
class ReadPPM : public BitmapReader
{
public:

    /**
     * @brief   Trys to parse the given file
     */
    ReadPPM(string filename);

    /**
     * @brief   Dtor.
     */
    virtual ~ReadPPM() {};

private:

    /**
     * @brief   Reads a new line from the given stream that is no
     *          comment
     *
     * @param   in      The stream to read from
     * @param   buffer  The extracted information
     */
    void readLine(ifstream & in, char* buffer);

};


}
#endif /* READPPM_HPP_ */
