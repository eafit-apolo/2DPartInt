#include <cstdlib>
#include <cmath>
#include <iostream>
extern "C" {
  #include "config.h"
  #include "csv.h"
  #include "data.h"
  #include "functions.h"
  #include "initialization.h"
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
  if (argc != 3) {
    std::cerr << "Wrong number of arguments: "
              << argc - 1
              << std::endl
              << "Usage: 2DPartInt [simulation_config_file] [output_folder]"
              << std::endl;
    return -1;
  }

  // Ensure the output folder exists.
  const char *output_folder = argv[2];
  if (ensure_output_folder(output_folder) != 0) {
    std::cerr << "The output folder does not exists, "
              << "and could not be created: "
              << output_folder
              << std::endl;
    return -1;
  }

  // Parse the config file.
  Config *config = new Config;
  parse_config(argv[1], config);

  // Initialize the simulation data structures.
  const size_t num_particles = initialize(config);

  // Write the initial state of the simulation.
  write_simulation_step(num_particles, particles, output_folder, 0);

  // Run the simulation until the max number of steps is reached.
  // The simulation time and the dt determine the maximum number of steps to execute.
  const unsigned long max_steps = ceil(config->simulation_time / config->dt);
  for (unsigned long step = 1; step <= max_steps; ++step) {
    simulation_step(num_particles, config->dt);
    write_simulation_step(num_particles, particles, output_folder, step);
  }

  // Free all memory resources and exit.
  delete config;
  free_all();
  return 0;
}
