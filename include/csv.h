#pragma once

#include "data.h"

/**
 * Ensures the output folder exists
 * Note: If not, it tries to create it.
 */
int ensure_output_folder(const char *output_folder);

/**
 * Writes a CSV file that can be read by ParaView,
 * with the current status of the simulation.
 * The file will be written on the specified folder,
 * and suffixed with the step number.
 */
void write_simulation_step(const size_t num_particles, const Particle *particles,
                           const char *folder, const unsigned long step);
