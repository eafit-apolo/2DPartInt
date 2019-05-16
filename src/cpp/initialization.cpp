#include <random>
extern "C" {
  #include "initialization.h"
  #include "config.h"
  #include "data.h"
  #include "functions.h"
}

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

  // Particles radius generator in milimeters
  std::mt19937 generator(config->seed);
  std::uniform_real_distribution<double> uniform_distribution(min_radius, max_radius);

  const unsigned int max_in_x = floor(config->m / diameter);
  double current_radius = uniform_distribution(generator);
  double last_radius;
  // Height + radius of particle below the particle currently positioned
  double particles_last_height[max_in_x] = {0};
  double x = current_radius;
  double y = current_radius;
  for (size_t i = 1; i < num_particles; ++i) {

    particles[i].x_coordinate = x;
    particles[i].y_coordinate = y;
    particles[i].radius = current_radius;
    properties[i].mass = config->mass;
    properties[i].kn = config->kn;
    properties[i].ks = config->ks;

    // Variables update
    last_radius = current_radius;
    current_radius = uniform_distribution(generator);
    y = particles_last_height[i % max_in_x] + current_radius;
    particles_last_height[i % max_in_x] = current_radius + particles[i].y_coordinate;

    // Check if this particle is the last one for this row...
    if ((i % max_in_x) == 0) {
      // If it is, reset the x value
      x = current_radius;
    } else {
      // Else, increment the x value.
      x += (last_radius + current_radius);
    }
  }

  // Initialize the falling particle.
  const double height = config->n * 80;

  particles[0].radius = uniform_distribution(generator);
  particles[0].x_coordinate = config->m / 2;
  particles[0].y_coordinate = height;
  properties[0].mass = config->mass;
  properties[0].kn = config->kn;
  properties[0].ks = config->ks;
  velocities[0].y_component = config->v0;

  // Return the number of initialized particles.
  return num_particles;
}
