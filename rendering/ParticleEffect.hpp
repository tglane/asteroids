/**
 * @file    ParticleEffect.hpp
 * @author  Timo Grootoonk
 * @date    Februar 2012
 */

#ifndef __ParticleEffect_HPP__
#define __ParticleEffect_HPP__

#include "Renderable.hpp"
#include "Particle.hpp"
#include "util/ColorMap.hpp"
#include "math/Vector.hpp"

#include <cstdlib>
#include <cmath>
#include <memory>
#include <list>

#ifdef __APPLE__
#include <OpenGL/gl.h>         
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

/**
 * @brief Class to manage different particle Effects.
 */
class ParticleEffect : public Renderable
{
private:
  /**
   * @brief Private Ctor. Create instances using the provide effect creation methods
   */
  ParticleEffect ( );
  
  static ColorMap  m_colorMap;

public:

  using Ptr = std::shared_ptr<ParticleEffect>;

  /**
   * @brief Dtor.
   */
  ~ParticleEffect ( );
  
  /// Particle array for the effect
  std::list<Particle::Ptr>   m_particles;
  
  /// Number of particles
  int                       m_count;
  
  /// Size of the particles
  float                     m_size;
  
  /// Update position and lifetime of each particle
  /// Returns true if lifetime of all particles is over
  bool update();

  /**
   * @brief     Creates a partile tail for bullets
   * 
   * @param     pos     Startposition of the buller
   * @param     speed   Speedvector
   * @param     lifetime
   * @return    A pointer to a new ParticleEffect instance
   **/
  static ParticleEffect::Ptr createBulletTail( Vector<float> pos, Vector<float> speed, int lifetime);
  
  /**
   * @brief     Creates an explosion sphere at the given position
   * 
   * @param     pos     Origin of an explosion
   * @return    A pointer to a new ParticleEffect instance
   */
  static ParticleEffect::Ptr createExplosionSphere( Vector<float> pos);
  
  /**
   * @brief     Creates an explosion ring at the given position
   * 
   * @param     pos     Origin of an explosion
   * @return    A pointer to a new ParticleEffect instance
   */
  static ParticleEffect::Ptr createExplosionRing( Vector<float> pos);
  
  /**
   * @brief     Renders the particle effect.
   */
  void render() override;


};

} // namespace asteroids

#endif // __ParticleEffect_HPP__
