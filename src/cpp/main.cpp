#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
extern "C" {
  #include "functions.h"
  #include "data.h"
}
#include "config.h"
#include "csv.h"
#include "initialization.h"

#ifdef DEBUG_STEP
#include "debug.h"
// Useful to keep track of the contacts buffer size.
size_t current_contacts_size;
size_t current_particle_index;
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

  // Reset forces to zeros.
  memset(forces, 0, sizeof(Vector) * particles_size);

  for (size_t part = 0; part < particles_size; ++part) {
    size_t contacts_size = compute_contacts(particles_size, particles, part, contacts_buffer);

    compute_forces(dt, particles_size, part, contacts_size, particles, properties,
                   contacts_buffer, velocities, normal_forces, tangent_forces, forces);
    compute_acceleration(part, properties, forces, accelerations);
    compute_velocity(dt, part, accelerations, velocities);
    compute_displacement(dt, part, velocities, displacements);
    displace_particle(part, displacements, particles);
    fix_displacement(part, velocities, particles);

#ifdef DEBUG_STEP
    current_contacts_size = contacts_size;

    // PROBLEM WITH THIS, always printig 300
    current_particle_index = part;
#endif
  }
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
  unsigned long debug_step;
  size_t debug_particle_index;
  std::cout << "Enter the simulation step number to debug: ";
  std::cin >> debug_step;
  if (debug_step > max_steps) {
    std::cerr << "The simulation step you provided is too big." << std::endl;
    return -1;
  }
  std::cout << "Enter the particle number to debug: ";
  std::cin >> debug_particle_index;
#endif

  for (unsigned long step = 1; step <= max_steps; ++step) {
    simulation_step(num_particles, config->dt);
    write_simulation_step(num_particles, particles, output_folder, step);

    // Write current system state to files.
#ifdef DEBUG_STEP // Start of debug.
    std::cout << "Crr: " << current_particle_index << " dbg " << debug_particle_index << "\n";
    if (step == debug_step && current_particle_index == debug_particle_index) {
      const char *debug_folder = "./debug";
      if (ensure_output_folder(debug_folder) != 0) {
        std::cerr << "The debug output folder does not exists, "
                  << "and could not be created: "
                  << debug_folder
                  << std::endl;
        return -1;
      }

      write_debug_information(step, current_particle_index, current_contacts_size,
                              debug_folder);
    }
#endif // End of debug.
  }

  // Free all memory resources and exit.
  delete config;
  free_all();
  return 0;
}
