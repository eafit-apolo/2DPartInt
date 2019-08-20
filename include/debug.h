#pragma once
#include <fstream>

/**
 * Writes the current step/state of the system - all its data structures -
 * to files in the specified folder.
 * If the number of the particle is not specified, it dumps all the particles out.
 */
void write_debug_information(const int step, const int particle_num,
                             const size_t num_particles, const size_t contacts_size,
                             const char *debug_folder);

/**
 * Writes the headear of a file given the data structure name that it represents
 * and the attributes it contains.
 */
void write_header(const int step, const int particle_num,
                  const char *data_structure_name,
                  const char *file_header,
                  std::ofstream &file);

/**
 * Writes the value of each data structure to each file that corresponds to
 * the particle indexed by 'index'.
 */
void write_values(const int index);

/**
 * Opens the needed files.
 * NOTE: If one of the files already exists, it erases its content.
 */
void open_files(const int size, const int particle_num, const char *debug_folder);

/**
 * Closes the already opened files.
 */
void close_files();
