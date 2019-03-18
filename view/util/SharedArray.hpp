/*
 *  SharedArray.hpp
 *
 *  @date 02.12.2018
 *  @author Thomas Wiemann
 * 
 *  Copyright (c) 2018 Thomas Wiemann.
 * 
 *  Restricted usage. Licensed for participants of the 
 *  course "The C++ Programming Language" only.
 * 
 *  No unauthorized distribution.
 */

#ifndef __SHAREDARRAY_HPP__
#define __SHAREDARRAY_HPP__

#include <memory>

namespace asteroids
{

/// Adator implementation to support shared arrays
/// with std::shared_ptr in C++11
template<typename T>
class shared_array : public std::shared_ptr<T>
{
public:

    /// Constructor. Creates a new shared pointer 
    /// with a custom deleter for arrays (implemented 
    /// with a corresponding lambda function)
    shared_array(T* array = 0) : 
        std::shared_ptr<T>(array, [](T* arr){delete[] arr;}) {}

    /// Returns a reference to the accessed array member
    T& operator[](int i) { return this->get()[i]; }

    /// Returns a copy of the accessed array member
    T operator[](int i) const { return this->get()[i]; } 
};

} // namespase asteroids
#endif