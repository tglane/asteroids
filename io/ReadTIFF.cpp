/*
 * ReadTIFF.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "ReadTIFF.hpp"
#include "tiffio.h"

namespace asteroids
{

ReadTIFF::ReadTIFF(string filename)
{

    TIFF* tif = TIFFOpen(filename.c_str(), "r");
    if (tif) {
        uint32 w, h;
        size_t npixels;
        uint32* raster;

        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        npixels = w * h;
        raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL) {
            if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {

            }
            _TIFFfree(raster);
        }
        TIFFClose(tif);
    }

}

ReadTIFF::~ReadTIFF()
{
    // TODO Auto-generated destructor stub
}


}

