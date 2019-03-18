
#include "ParticleEffect.hpp"
#include "math/Randomizer.hpp"

#include <iostream>
using namespace std;

namespace asteroids
{

ColorMap ParticleEffect::m_colorMap = ColorMap(400);

ParticleEffect::ParticleEffect()
{
  /// Initialize random number generator
  srand(time(0));
}

ParticleEffect::~ParticleEffect()
{
}

ParticleEffect::Ptr ParticleEffect::createBulletTail(Vector<float> pos, Vector<float> speed, int lifetime)
{
  /// Neue Instanz
  ParticleEffect::Ptr pi = std::make_shared<ParticleEffect>(ParticleEffect());

  /// 40 Schreifpartikel
  pi->m_count = 40;
  for (int i = 0; i < pi->m_count; i++)
  {
    pi->m_particles.push_back(make_shared<Particle>(Particle()));
  }

  /// Differenzwert
  float dif;

  /// Array initialisieren
  for (auto p : pi->m_particles)
  {
    /// Position mit Unschaerfe
    dif = Randomizer::instance()->getRandomNumber(-10, 10);
    p->m_pos[0] = pos[0] + dif;
    p->m_pos[1] = pos[1] + dif;
    p->m_pos[2] = pos[2] + dif;

    /// Geschwindigkeit mit Unschaerfe
    dif = Randomizer::instance()->getRandomNumber(0, 10);

    p->m_speed[0] = speed[0] * dif;
    p->m_speed[1] = speed[1] * dif;
    p->m_speed[2] = speed[2] * dif;

    /// Farbe
    p->m_color[0] = 0.1;
    p->m_color[1] = 1.0;
    p->m_color[2] = 0.1;

    /// Lebenszeit
    p->m_lifetime = lifetime;
  }
  pi->m_size = 2.0;

  /// Instanz zurueckliefern
  return pi;
}

ParticleEffect::Ptr ParticleEffect::createExplosionSphere(Vector<float> pos)
{
  /// Neue Partikelinstanz
  ParticleEffect::Ptr pi = make_shared<ParticleEffect>(ParticleEffect());

  /// 300 Partikel
  pi->m_count = 400;
  for (int i = 0; i < pi->m_count; i++)
  {
    pi->m_particles.push_back(make_shared<Particle>(Particle()));
  }

  /// Array initialisieren
  int i = 0;
  for (auto p : pi->m_particles)
  {

    float phi = Randomizer::instance()->getRandomNumber(0, 4);
    float theta = Randomizer::instance()->getRandomNumber(0, 2);

    Vector<float> speed(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    float speed_scale = Randomizer::instance()->getRandomNumber(0, 2);
    speed = speed * speed_scale;

    p->m_pos = pos;
    p->m_speed = speed;

    float color[3];
    m_colorMap.getColor(&color[0], i, HOT);

    p->m_color[0] = color[0];
    p->m_color[1] = color[1];
    p->m_color[2] = color[2];

    p->m_lifetime = Randomizer::instance()->getRandomNumber(0, 400);
    i++;
  }
  pi->m_size = 2.0;
  /// Instanz zurueckliefern
  return pi;
}

ParticleEffect::Ptr ParticleEffect::createExplosionRing(Vector<float> pos)
{
  /// Neue Instanz
  ParticleEffect::Ptr pi = std::make_shared<ParticleEffect>(ParticleEffect());

  /// 40 Schreifpartikel
  pi->m_count = 300;
  for (int i = 0; i < pi->m_count; i++)
  {
    pi->m_particles.push_back(make_shared<Particle>(Particle()));
  }

  /// Array initialisieren
  for (auto p : pi->m_particles)
  {
    /// Zufallsposition
    p->m_pos[0] = 2.0 * rand() / RAND_MAX - 1;
    p->m_pos[1] = 2.0 * rand() / RAND_MAX - 1;
    p->m_pos[2] = 2.0 * rand() / RAND_MAX - 1;
    p->m_pos.normalize();
    /// Position mit Unschaerfe
    p->m_pos = pos + p->m_pos * 10;

    /// Zufallsgeschwindigkeitsvektor (als Ring)
    p->m_speed[0] = 1.0 * rand() / RAND_MAX - 1;
    p->m_speed[1] = 1.0 * rand() / RAND_MAX - 1;
    p->m_speed[2] = 0;
    p->m_speed.normalize();

    /// Geschwindigkeitsvektor mit Unschaerfe
    p->m_speed = p->m_speed * (80.0 * rand() / RAND_MAX + 60);

    /// Farbe weiss
    p->m_color[0] = 1.0;
    p->m_color[1] = 1.0;
    p->m_color[2] = 1.0;

    /// Lebenszeit
    p->m_lifetime = (500.0 * rand() / RAND_MAX);
  }
  pi->m_size = 2.0;
  /// Instanz zurueckliefern
  return pi;
}

bool ParticleEffect::update()
{

  bool dead = true;
  for (auto p : m_particles)
  {
    // Check if particle is alive
    if (p->m_lifetime > 0)
    {
      dead = false;
    }
    p->m_pos = p->m_pos + p->m_speed;
    p->m_lifetime--;
  }
  return dead;
}

void ParticleEffect::render()
{
  /// Positionspunkte
  float p1, p2, p3, p4, p5, p6;

  /// Position z und Groesse des Partikels
  float z;

  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_TEXTURE);
  glDisable(GL_LIGHTING);
  // Draw particles as quads
  glBegin(GL_QUADS);
  for (auto p : m_particles)
  {
    // Check lifetime
    if (p->m_lifetime >= 0)
    {

      z = p->m_pos[2];
      p1 = p->m_pos[0] + m_size;
      p2 = p->m_pos[1] + m_size;
      p3 = z + m_size;
      p4 = p->m_pos[0] - m_size;
      p5 = p->m_pos[1] - m_size;
      p6 = z - m_size;

      glColor3f(p->m_color[0], p->m_color[1], p->m_color[2]);

      glVertex3f(p1, p2, p3);
      glVertex3f(p1, p2, p6);
      glVertex3f(p4, p5, p6);
      glVertex3f(p4, p5, p3);

      glVertex3f(p4, p2, p3);
      glVertex3f(p4, p2, p6);
      glVertex3f(p1, p5, p6);
      glVertex3f(p1, p5, p3);

      glVertex3f(p1, p2, z);
      glVertex3f(p4, p2, z);
      glVertex3f(p4, p5, z);
      glVertex3f(p1, p5, z);
    }
  }
  /// Zeichnen Ende
  glEnd();
  glEnable(GL_TEXTURE);
  glEnable(GL_LIGHTING);
  glPopAttrib();
}

} // namespace asteroids
