#include <fstream>
extern "C" {
  #include "data.h"
}

// Simulation data structures.
extern Particle *particles;
extern ParticleProperties *properties;
extern Contact *contacts_buffer;
extern double *normal_forces;
extern double *tangent_forces;
extern Vector *forces;
extern Vector *accelerations;
extern Vector *velocities;
extern Vector *displacements;

void write_debug_information(const size_t step, const size_t particle_num,
                             const size_t num_particles, const size_t contacts_size,
                             const char *debug_folder) {

  std::string file_name(std::string(debug_folder) + "/"
                        + std::to_string(step) + "_"
                        + std::to_string(particle_num) + "_");
  std::ofstream particles_file(file_name + "particles.debug");
  std::ofstream properties_file(file_name + "properties.debug");
  std::ofstream contacts_file(file_name + "contacts.debug");
  std::ofstream normal_forces_file(file_name + "normal_forces.debug");
  std::ofstream tangent_forces_file(file_name + "tangent_forces.debug");
  std::ofstream forces_file(file_name + "forces.debug");
  std::ofstream accelerations_file(file_name + "accelerations.debug");
  std::ofstream velocities_file(file_name + "velocities.debug");
  std::ofstream displacements_file(file_name + "displacements.debug");

  // Write the contents of each data structure to its corresponding file.

  particles_file.close();
  properties_file.close();
  contacts_file.close();
  normal_forces_file.close();
  tangent_forces_file.close();
  forces_file.close();
  accelerations_file.close();
  velocities_file.close();
  displacements_file.close();
}
