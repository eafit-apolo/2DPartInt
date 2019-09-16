#include <cstdlib>
#include <cmath>
#include <iostream>
extern "C" {
  #include "functions.h"
  #include "data.h"
}
#include "config.h"
#include "csv.h"
#include "initialization.h"

// Debug needed definitions.
#ifdef DEBUG_STEP
// Useful to keep track of the contacts buffer size.
size_t debug_contacts_size;
// Step number and particle number to debug.
unsigned long debug_step;
size_t debug_particle_number;
#endif

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

// Current simulation step.
unsigned long step;

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

#ifdef DEBUG_STEP
  debug_contacts_size = contacts_size;
#endif

  compute_forces(dt, particles_size, contacts_size, particles, properties,
                 contacts_buffer, velocities, normal_forces, tangent_forces, forces);
  compute_acceleration(particles_size, properties, forces, accelerations);
  compute_velocity(dt, particles_size, accelerations, velocities);
  compute_displacement(dt, particles_size, velocities, displacements);
  displace_particles(particles_size, displacements, particles);
  fix_displacements(particles_size, velocities, particles);
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

#ifdef DEBUG_STEP
  // Receive simulation step as input.
  std::cout << "Enter the debug information\n";
  std::cout << "Enter the simulation step number: ";
  std::cin >> debug_step;
  std::cout << "Enter the particle number: ";
  std::cin >> debug_particle_number;

  if (debug_step > max_steps || debug_particle_number >= num_particles) {
    std::cerr << "DEBUG ERROR\n";
    std::cerr << "\tDebug step given: " << debug_step
              << ". Maximum allowed: " << max_steps << "\n";
    std::cerr << "\tDebug particle number given: " << debug_particle_number
              << ". Maximum allowed: " << num_particles-1 << "\n";
    return -1;
  }
#endif

  for (step = 1; step <= max_steps; ++step) {
    // Track particle calculations.
    simulation_step(num_particles, config->dt);
    write_simulation_step(num_particles, particles, output_folder, step);

#ifdef DEBUG_STEP
    if (step == debug_step) break;
#endif
  }

  // Free all memory resources and exit.
  delete config;
  free_all();
  return 0;
}
