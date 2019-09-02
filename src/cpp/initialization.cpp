#include <random>
extern "C" {
  #include "functions.h"
  #include "data.h"
}
#include "initialization.h"
#include "config.h"

// Simulation data structures.
extern Particle *particles;
extern ParticleProperties *properties;
extern Contact *contacts_buffer;
extern double *normal_forces;
extern double *tangent_forces;
extern Vector *forces;
extern Vector *accelerations;
extern Vector *velocities;
extern Vector *displacements;

#ifndef M_PI
  #define M_PI 3.141592653589793
#endif

/**
 * Computes the mass of a particle, given its radius.
 */
double compute_mass(const Config *config, const double radius) {
  return config->rho * config->thickness * M_PI * radius * radius;
}

/**
 * Initialize all simulation data structures,
 * according to the simulation size.
 * Returns the number of initialized particles.
 *
 * Note: Except for the particles,
 * all structures are effectively initialized with zeros.
 */
size_t initialize(const Config *config) {
  const double min_radius = 10.0d;
  const double max_radius = 50.0d;
  const double diameter = 2 * max_radius;

  // The number of particles is equals to the simulation size,
  // divided by the size of each particle,
  // plus the falling particle (the first one).
  const size_t num_particles = floor((config->m * config->n) / (diameter * diameter)) +  1;

  // Allocate the memory for all the data structures.
  particles = (Particle*) calloc(num_particles, sizeof(Particle));
  properties = (ParticleProperties*) calloc(num_particles, sizeof(ParticleProperties));
  contacts_buffer = (Contact*) calloc(size_triangular_matrix(num_particles), sizeof(Contact));
  normal_forces = (double*) calloc(num_particles * num_particles, sizeof(double));
  tangent_forces = (double*) calloc(num_particles * num_particles, sizeof(double));
  forces = (Vector*) calloc(num_particles, sizeof(Vector));
  accelerations = (Vector*) calloc(num_particles, sizeof(Vector));
  velocities = (Vector*) calloc(num_particles, sizeof(Vector));
  displacements = (Vector*) calloc(num_particles, sizeof(Vector));

  // Initialize the particles.

  // Particles radius generator in millimeters.
  std::mt19937 generator(config->seed);
  std::uniform_real_distribution<double> uniform_distribution(min_radius, max_radius);

  double current_radius = 150;//uniform_distribution(generator);
  double last_radius;
  double x = current_radius;
  double y = current_radius;
  for (size_t i = 1; i < num_particles; ++i) {
    particles[i].x_coordinate = x;
    particles[i].y_coordinate = y;
    particles[i].radius = current_radius;
    properties[i].mass = compute_mass(config, current_radius);
    properties[i].kn = config->kn;
    properties[i].ks = config->ks;

    // Variables update
    last_radius = current_radius;
    current_radius = 150; //uniform_distribution(generator);
    x += (last_radius + current_radius);

    // Check if the x coordinate overpasses the width of the simulation
    if ((x + max_radius) > config->m) {
      // If it does, reset it
      x = current_radius;
      y += diameter;
    }

  }

  // Initialize the falling particle.
  particles[0].radius = config->r0;
  particles[0].x_coordinate = config->m / 2;
  particles[0].y_coordinate = config->n + (max_radius * 2);
  properties[0].mass = compute_mass(config, config->r0);
  properties[0].kn = config->kn;
  properties[0].ks = config->ks;
  velocities[0].y_component = config->v0;

  // Return the number of initialized particles.
  return num_particles;
}
