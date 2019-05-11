#include <cstdio>
#include <cstdlib>
#include <cmath>
extern "C" {
  #include "config.h"
  #include "data.h"
  #include "functions.h"
}

// Simulation data structures.
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
 * Initialize all simulation data structures,
 * according to the simulation size.
 * Returns the number of initialized particles.
 *
 * Note: Except for the particles,
 * all structures are effectively initialized with zeros.
 */
size_t initialize(const Config *config) {
  const double diameter = 2 * config->r;

  // The number of particles is equals to the simulation size,
  // divided by the size of each particle,
  // plus the falling particle (the first one).
  const size_t num_particles = floor((config->m * config->n) / (2 * diameter)) + 1;

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
  const unsigned int max_in_x = floor(config->m / diameter);
  double x = config->r;
  double y = config->r;
  for (size_t i = 1; i < num_particles; ++i) {
    particles[i].x_coordinate = x;
    particles[i].y_coordinate = y;
    particles[i].radious = config->r;
    properties[i].mass = config->mass;
    properties[i].kn = config->kn;
    properties[i].ks = config->ks;

    // Check if this particle is the last one for this row...
    if ((i % max_in_x) == 0) {
      // If it is, reset the x value and increment y.
      x = config->r;
      y += diameter;
    } else {
      // If not, increment the x value.
      x += diameter;
    }
  }

  // Initialize the falling particle.
  particles[0].x_coordinate = config->m / 2;
  particles[0].y_coordinate = config->n + (4 * config->r);
  particles[0].radious = config->r;
  properties[0].mass = config->mass;
  properties[0].kn = config->kn;
  properties[0].ks = config->ks;
  velocities[0].x_component = config->v0;

  // Return the number of initialized particles.
  return num_particles;
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
    fprintf(
      stderr,
      "Wrong number of arguments: %d\nUsage: 2DPartInt [simulation_config_file]\n",
      argc - 1
    );
    return -1;
  }

  // Parse the config file.
  Config *config = new Config;
  parse_config(argv[1], config);

  // Initialize the simulation data structures.
  size_t num_particles = initialize(config);

  // Run the simulation until the max number of steps is reached.
  // The simulation time and the dt determine the maximum number of steps to execute.
  const unsigned int max_steps = ceil(config->simulation_time / config->dt);
  for (unsigned int step = 0; step < max_steps; ++step) {
    simulation_step(num_particles, config->dt);
  }

  // Free all memory resources and exit.
  free(config);
  free_all();
  return 0;
}
