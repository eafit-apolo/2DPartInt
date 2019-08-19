/**
 * Writes the current step/state of the system - all its data structures -
 * to files in the specified folder.
 * If the number of the particle is not specified, it dumps all the particles out.
 */
void write_debug_information(const size_t step, const size_t particle_num,
                             const size_t num_particles, const size_t contacts_size,
                             const char *debug_folder);
