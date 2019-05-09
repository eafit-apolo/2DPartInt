#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "data.h"
#include "functions.h"

// Simulation structures.
Particle *particles;
ParticleProperties *properties;
Contact *contacts_buffer;
double *normal_forces;
double *tangent_forces;
Vector *forces;
Vector *accelerations;
Vector *velocities;
Vector *displacements;

/**
 * Initialize all simulation structures according to the number of particles.
 * The structures are effectively initialized with zeros.
 */
void initialize(const Config *config) {
  // The number of particles is equals to the simulation size,
  // plus the falling particle (the first one).
  size_t num_particles = config->simulation_size + 1;

  // Allocate memory.
  particles = calloc(num_particles, sizeof(Particle));
  properties = calloc(num_particles, sizeof(ParticleProperties));
  contacts_buffer = calloc(size_triangular_matrix(num_particles), sizeof(Contact));
  normal_forces = calloc(num_particles * num_particles, sizeof(double));
  tangent_forces = calloc(num_particles * num_particles, sizeof(double));
  forces = calloc(num_particles, sizeof(Vector));
  accelerations = calloc(num_particles, sizeof(Vector));
  velocities = calloc(num_particles, sizeof(Vector));
  displacements = calloc(num_particles, sizeof(Vector));

  // Initialize the structures.
  for (size_t i = 0; i < num_particles; ++i) {
    particles[i].radious = config->r;
    properties[i].mass = config->m;
    properties[i].kn = config->kn;
    properties[i].ks = config->ks;
  }
  velocities[0].x_component = config->v0;
}

/**
 * Free all the structures allocated by initialize.
 */
void free_all() {
  free(particles);
  free(properties);
  free(contacts_buffer);
  free(normal_forces);
  free(tangent_forces);
  free(forces);
  free(accelerations);
  free(velocities);
  free(displacements);
}

/**
 * Executes one step of the simulation.
 */
void simulation_step(const size_t particles_size, const double dt) {
  size_t contacts_size = compute_contacts(particles_size, particles, contacts_buffer);
  compute_forces(dt, particles_size, contacts_size, particles, properties,
                 contacts_buffer, velocities, normal_forces, tangent_forces, forces);
  compute_acceleration(particles_size, properties, forces, accelerations);
  compute_velocity(dt, particles_size, accelerations, velocities);
  compute_displacement(dt, particles_size, velocities, displacements);
  displace_particles(particles_size, displacements, particles);
}

/**
 * Main method - All code logic runs here.
 */
int main(int argc, char *argv[]) {
  // Ensure the program was called with the correct number of arguments.
  if (argc != 2) {
    fprintf(stderr, "Wrong number of arguments: %d\nUsage: 2DPartInt [simulation_config_file]", argc);
    return -1;
  }

  Config *config = malloc(sizeof(Config));
  parse_config(argv[1], config);
  initialize(config);
  const int max_steps = ceil(config->simulation_time / config->dt);
  const size_t num_particles = config->simulation_size + 1;

  // Run the simulation until the max number of steps is reached.
  for (int step = 0; step < max_steps; ++step) {
    simulation_step(num_particles, config->dt);
  }

  // Free all memory resources and exit.
  free(config);
  free_all();
  return 0;
}
