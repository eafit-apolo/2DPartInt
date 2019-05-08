#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

// Simulation structures.
Particle *particles;
ParticleProperties *properties;
Contact *contacts_buffer;
Vector *forces;
Vector *accelerations;
Vector *velocities;
Vector *displacements;

/**
 * Initialize all simulation structures according to the number of particles.
 * The structures are effectively initialized with zeros.
 */
void initialize(const size_t num_particles) {
  // Allocate memory.
  particles = calloc(num_particles, sizeof(Particle));
  properties = calloc(num_particles, sizeof(ParticleProperties));
  contacts_buffer = calloc(size_triangular_matrix(num_particles), sizeof(Contact));
  forces = calloc(num_particles, sizeof(Vector));
  accelerations = calloc(num_particles, sizeof(Vector));
  velocities = calloc(num_particles, sizeof(Vector));
  displacements = calloc(num_particles, sizeof(Vector));
}

/**
 * Free all the structures allocated by initialize.
 */
void free_all() {
  free(particles);
  free(properties);
  free(contacts_buffer);
  free(forces);
  free(accelerations);
  free(velocities);
  free(displacements);
}

/**
 * Main method - All code logic runs here.
 */
int main(void) {
  // The simulation size is equals to the number of particles.
  const size_t simulation_size = 100;
  const double dt = 0.000025;
  initialize(simulation_size);

  // Free all memory resources and exit.
  free_all();
  return 0;
}
