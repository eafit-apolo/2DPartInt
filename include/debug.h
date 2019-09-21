#pragma once
#include <fstream>

/**
 * Writes the current step/state of the system - all the data structures that represent a particle -
 * to a file in a specified folder.
 */
void write_debug_information(const int step, const size_t particle_index,
                             const size_t contacts_size, const char *debug_folder);

/**
 * Writes the headear of a file given the data structure name that it represents
 * and the attributes it contains.
 */
void write_header(const int step, std::ofstream &file);

/**
 * Writes the value of each data structure to a file.
 */
void write_values(const size_t particle_index, const size_t contacts_size, std::ofstream &file);
