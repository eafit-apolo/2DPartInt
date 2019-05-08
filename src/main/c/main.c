#include <math.h>
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
 * Executes one step of the simulation.
 */
void simulation_step(const size_t particles_size, const double dt) {
  size_t contact_size = compute_contacts(particles_size, particles, contacts_buffer);
  //compute_forces(particles_size, contacts_size, contacts_buffer, particles, properties, velocities, dt, forces);
  compute_acceleration(particles_size, properties, forces, accelerations);
  compute_velocity(particles_size, accelerations, dt, velocities);
  compute_displacement(particles_size, dt, velocities, displacements);
  displace_particles(particles_size, displacements, particles);
}

/**
 * Main method - All code logic runs here.
 */
int main(void) {
  // The simulation size is equals to the number of particles.
  const size_t simulation_size = 100;
  initialize(simulation_size);
  // The simulation time and dt determine the maximum number of steps to execute.
  const double simulation_time = 10;
  const double dt = 0.000025;
  const int max_steps = ceil(simulation_time / dt);

  // Run the simulation until the max number of steps is reached.
  for (int step = 0; step < max_steps; ++step) {
    simulation_step(simulation_size, dt);
  }

  // Free all memory resources and exit.
  free_all();
  return 0;
}
