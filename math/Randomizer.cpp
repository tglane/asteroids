/*
 *  Randomizer.cpp
 *
 *  @date 18.11.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "Randomizer.hpp"

namespace asteroids
{

Randomizer* Randomizer::p_instance = 0;

Randomizer* Randomizer::instance()
{
    if(!Randomizer::p_instance)
    {
        srand(time(0));
        Randomizer::p_instance = new Randomizer;
    }
    return Randomizer::p_instance;
}

float Randomizer::getRandomNumber(float a, float b)
{
    double r = b - a + 1;
    return a + (r * rand()/(RAND_MAX+1.0));
}

Vector3f Randomizer::getRandomVertex(float range)
{
    return Vector3f(getRandomNumber(-range, range),
                         getRandomNumber(-range, range),
                         getRandomNumber(-range, range));
}


} /* namespace asteroids */
