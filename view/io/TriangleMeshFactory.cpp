/*
 * TraingleMeshFactory.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */
#include <iostream>
using std::cout;
using std::endl;

#include "TriangleMeshFactory.hpp"

#include "io/Read3DS.hpp"
#include "io/ReadPLY.hpp"
#include "io/ReadOBJ.hpp"

namespace asteroids
{

auto TriangleMeshFactory::getMesh(const string &filename) const -> TriangleMesh::Ptr
{
    // A mesh pointer and MeshIO pointer
    MeshReader* io = 0;
    TriangleMesh::Ptr mesh = 0;

    // Get file extension
    if(filename.substr(filename.find_last_of(".") + 1) == "ply")
    {
        io = new ReadPLY(filename);
    }
    else if(filename.substr(filename.find_last_of(".") + 1) == "3ds")
    {
        io = new Read3DS(filename);
    }
    else if(filename.substr(filename.find_last_of(".") + 1) == "obj")
    {
        io = new ReadOBJ(filename);
    }

    // Get mesh from io
    if(io)
    {
        mesh = io->getMesh();
    }
    else
    {
        cout << "Unable to parse file " << filename << "." << endl;
    }
    
    delete io;
    return mesh;
}

TriangleMeshFactory& TriangleMeshFactory::instance()
{
    // Just crate on instance
    static TriangleMeshFactory instance;
    return instance;
}

}
