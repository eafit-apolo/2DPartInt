#include <fstream>
extern "C" {
  #include "data.h"
}
#include "debug.h"

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

// Output file streams.
std::ofstream particles_file;
std::ofstream properties_file;
std::ofstream contacts_file;
std::ofstream normal_forces_file;
std::ofstream tangent_forces_file;
std::ofstream forces_file;
std::ofstream accelerations_file;
std::ofstream velocities_file;
std::ofstream displacements_file;

void write_debug_information(const int step, const int particle_num,
                             const size_t num_particles, const size_t contacts_size,
                             const char *debug_folder) {

  open_files(step, particle_num, debug_folder);

  // Write the contents of each data structure to its corresponding file.
  // Write the header of each file.
  write_header(step, particle_num, "Particles.",
               "x_coordinate y_coordinate raidus", particles_file);
  write_header(step, particle_num, "Particle Properties.",
               "mass kn ks", properties_file);
  write_header(step, particle_num, "Contacts.",
               "p1_idx p2_idx overlap", contacts_file);
  write_header(step, particle_num, "Normal forces.",
               "", normal_forces_file);
  write_header(step, particle_num, "Tangent forces.",
               "", tangent_forces_file);
  write_header(step, particle_num, "Forces.",
               "x_component y_component", forces_file);
  write_header(step, particle_num, "Accelerations.",
               "x_component y_component", accelerations_file);
  write_header(step, particle_num, "Velocities.",
               "x_component y_component", velocities_file);
  write_header(step, particle_num, "Displacements.",
               "x_component y_component", displacements_file);

  // Write data structures content.
  if (particle_num != -1) // If particle specified.
    write_values(particle_num);
  else
    for (size_t i = 0; i < num_particles; ++i)
      write_values(i);

  // Write contacts data.
  for (size_t i = 0; i < contacts_size; ++i) {
    contacts_file << contacts_buffer[i].p1_idx << " "
                  << contacts_buffer[i].p2_idx << " "
                  << contacts_buffer[i].overlap << "\n";
  }

  close_files();
}

void write_header(const int step, const int particle_num,
                  const char *data_structure_name,
                  const char *file_header,
                  std::ofstream &file) {
  file << data_structure_name << " Iteration: " << step
       << ", Particle: "
       << ((particle_num != -1) ? std::to_string(particle_num).c_str() : "*") << "\n"
       << std::string(file_header) << "\n";
}

void write_values(const int index) {
    particles_file << particles[index].x_coordinate << " "
                   << particles[index].y_coordinate << " "
                   << particles[index].radius << "\n";

    properties_file << properties[index].mass << " "
                   << properties[index].kn << " "
                   << properties[index].ks << "\n";

    normal_forces_file << normal_forces[index] << "\n";
    tangent_forces_file << tangent_forces[index] << "\n";

    forces_file << forces[index].x_component << " "
                << forces[index].y_component << "\n";

    accelerations_file << accelerations[index].x_component << " "
                << accelerations[index].y_component << "\n";

    velocities_file << velocities[index].x_component << " "
                    << velocities[index].y_component << "\n";

    displacements_file << displacements[index].x_component << " "
                       << displacements[index].y_component << "\n";
}

void open_files(const int step, const int particle_num, const char *debug_folder) {
  std::string file_name(std::string(debug_folder) + "/"
                        + std::to_string(step) + "_"
                        + ((particle_num != -1) ? std::to_string(particle_num).c_str() : "*")
                        + "_");
  particles_file.open(file_name + "particles.debug",
                      std::ios::out | std::ios::trunc);
  properties_file.open(file_name + "properties.debug",
                       std::ios::out | std::ios::trunc);
  contacts_file.open(file_name + "contacts.debug",
                     std::ios::out | std::ios::trunc);
  normal_forces_file.open(file_name + "normal_forces.debug",
                          std::ios::out | std::ios::trunc);
  tangent_forces_file.open(file_name + "tangent_forces.debug",
                           std::ios::out | std::ios::trunc);
  forces_file.open(file_name + "forces.debug",
                   std::ios::out | std::ios::trunc);
  accelerations_file.open(file_name + "accelerations.debug",
                          std::ios::out | std::ios::trunc);
  velocities_file.open(file_name + "velocities.debug",
                       std::ios::out | std::ios::trunc);
  displacements_file.open(file_name + "displacements.debug",
                          std::ios::out | std::ios::trunc);
}

void close_files() {
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
