#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "math/Vector.hpp"

#include <memory>

namespace asteroids
{

/**
 * @brief Struct representing a single particle
 */
class Particle
{
public:

  using Ptr = std::shared_ptr<Particle>;

  /// Position
  Vector<float>    m_pos;
  
  /// Speed
  Vector<float>    m_speed;
  
  /// Color
  Vector<float>    m_color;
  
  /// Lifetime
  int         m_lifetime;
};



}

#endif // __PARTICLE_HPP__
