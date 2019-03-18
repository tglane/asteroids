/*
 * ReadJPG.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#include "ReadJPG.hpp"

namespace asteroids
{

ReadJPG::ReadJPG(string filename)
{
    int depth;
    
    FILE *fd;
   
    fd = fopen(filename.c_str(), "rb");
    
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    
    unsigned long location = 0;
    
    cinfo.err = jpeg_std_error(&jerr);
    
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fd);
    jpeg_read_header(&cinfo, (boolean)0);
    cinfo.scale_num = 1;
    cinfo.scale_denom = 1;
    jpeg_start_decompress(&cinfo);
    m_width = cinfo.output_width;
    m_height = cinfo.output_height;
    depth = cinfo.num_components; //should always be 3
    m_pixels = Ptr(new unsigned char[m_width * m_height * depth]);
    row_pointer[0] = new unsigned char[m_width * depth];
    
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.output_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for(int i =0 ; i < (m_width * depth); i++)
            m_pixels[location++] = row_pointer[0][i];
    }
    delete[] row_pointer[0];
    fclose(fd);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}


ReadJPG::~ReadJPG()
{

}

}
