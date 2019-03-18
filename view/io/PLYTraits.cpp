/*
 * PLYTraits.hpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */


#include "PLYTraits.hpp"

namespace asteroids
{

string PLYTraits<char>::NAME = "char";
size_t PLYTraits<char>::SIZE = 1;

string PLYTraits<uchar>::NAME = "uchar";
size_t PLYTraits<uchar>::SIZE = 1;

string PLYTraits<short>::NAME = "short";
size_t PLYTraits<short>::SIZE = 2;

string PLYTraits<ushort>::NAME = "ushort";
size_t PLYTraits<ushort>::SIZE = 2;

string PLYTraits<int>::NAME = "int";
size_t PLYTraits<int>::SIZE = 4;


string PLYTraits<uint>::NAME = "uint";
size_t PLYTraits<uint>::SIZE = 4;

string PLYTraits<float>::NAME = "float";
size_t PLYTraits<float>::SIZE = 4;

string PLYTraits<double>::NAME = "double";
size_t PLYTraits<double>::SIZE = 8;

}
