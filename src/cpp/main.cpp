#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
extern "C" {
  #include "functions.h"
  #include "data.h"
  #include "collisions.h"
}
#include "config.h"
#include "csv.h"
#include "initialization.h"

#ifdef DEBUG_STEP
#include "debug.h"
// Useful to keep track of the current simulation step.
unsigned long current_step;

// Step and particle to debug given by the user.
unsigned long step_to_debug;
size_t particle_to_debug;
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
Particle **grid;

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
  free(grid);
}

/**
 * Executes one step of the simulation.
 */
void simulation_step(const size_t particles_size, const double dt, const int x_squares, const int y_squares, const double squares_length, int step) {

  // Reset forces to zeros.
  memset(forces, 0, sizeof(Vector) * particles_size);
  memset(grid, 0, sizeof(Particle*) * x_squares * y_squares);

  fill_grid(particles_size, particles, grid, x_squares, y_squares, squares_length);
  size_t contacts_size = compute_contacts(grid, x_squares, y_squares, squares_length, contacts_buffer);
  compute_forces(dt, particles_size, contacts_size, particles, properties,
                 contacts_buffer, velocities, normal_forces, tangent_forces, forces);

  for (size_t part = 0; part < particles_size; ++part) {
    compute_acceleration(part, properties, forces, accelerations);
    compute_velocity(dt, part, accelerations, velocities);
    compute_displacement(dt, part, velocities, displacements);
    displace_particle(part, displacements, particles);
    fix_displacement(part, velocities, particles);

#ifdef DEBUG_STEP
    if (current_step == step_to_debug && part == particle_to_debug) {
      const char *debug_folder = "./debug";
      if (ensure_output_folder(debug_folder) != 0) {
        std::cerr << "The debug output folder does not exists, "
                  << "and could not be created: "
                  << debug_folder
                  << std::endl;
        exit(-1);
      }

      write_debug_information(step_to_debug, particle_to_debug,
                              contacts_size, debug_folder);
    }
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
  std::cout << "Enter the simulation step number to debug: ";
  std::cin >> step_to_debug;
  if (step_to_debug > max_steps) {
    std::cerr << "The simulation step you provided," << step_to_debug
              << ", won't be executed." << std::endl;
    return -1;
  }
  std::cout << "Enter the particle number to debug: ";
  std::cin >> particle_to_debug;
  if (particle_to_debug > num_particles) {
    std::cerr << "The particle index you provided, " << particle_to_debug
              << ", doesn't exist." << std::endl;
    return -1;
  }
#endif

  write_grid(config->x_squares, config->y_squares, config->square_in_grid_length, output_folder);

  for (unsigned long step = 1; step <= max_steps; ++step) {
#ifdef DEBUG_STEP
    current_step = step;
#endif

    simulation_step(num_particles, config->dt, config->x_squares, config->y_squares, config->square_in_grid_length, step);
    write_simulation_step(num_particles, particles, output_folder, step);
  }

  // Free all memory resources and exit.
  delete config;
  free_all();
  return 0;
}
