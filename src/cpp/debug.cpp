#include <fstream>
#include <iomanip>
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

void write_debug_information(const int step, const size_t num_particles,
                             const size_t contacts_size, const char *debug_folder) {

  std::ofstream debug_file(std::string(debug_folder) + "/"
                           + "step_" + std::to_string(step),
                      std::ios::out | std::ios::trunc);

  int hola = contacts_size;
  ++hola;
  // Write the header file. That is, columns for each data structure.
  write_header(step, debug_file);

  // Write data structures content.
  write_values(num_particles, contacts_size, debug_file);

  debug_file.close();
  exit(0); // Do not continue simulating.
}

void write_header(const int step, std::ofstream &file) {
  file << "SIMULATION STEP: " << step << "\n";
  int column_width = 11;
  // Particles data structure.
  file << std::setw(column_width) << "x_coor"
       << std::setw(column_width) << "y_coor";
  column_width = 7;
  file << std::setw(column_width) << "radius";
  column_width = 11;
  // Properties data structure.
  file << std::setw(column_width) << "mass"
       << std::setw(column_width) << "kn"
       << std::setw(column_width) << "ks";
  // Normal forces and tangent forces.
  column_width = 8;
  file << std::setw(column_width) << "norm_f"
       << std::setw(column_width) << "tang_f";
  column_width = 11;
  // Forces.
  file << std::setw(column_width) << "forc_x"
       << std::setw(column_width) << "forc_y"
  // Accelerations.
       << std::setw(column_width) << "accel_x"
       << std::setw(column_width) << "accel_y"
  // Velocities.
       << std::setw(column_width) << "vel_x"
       << std::setw(column_width) << "vel_y"
  // Displacements.
       << std::setw(column_width) << "disp_x"
       << std::setw(column_width) << "disp_y\n";
}

void write_values(const size_t num_particles, const size_t contacts_size,
                  std::ofstream &file) {
  int column_width = 11;
  const int precision = 4;
  for (size_t i = 0; i < num_particles; ++i) {
    file << std::setw(column_width) << std::setprecision(precision)
         << particles[i].x_coordinate
         << std::setw(column_width) << std::setprecision(precision)
         << particles[i].y_coordinate;
    column_width = 7;
    file<< std::setw(column_width) << std::setprecision(precision)
        << particles[i].radius;
    column_width = 11;
      // Properties information.
    file << std::setw(column_width) << std::setprecision(precision)
         << properties[i].mass
         << std::setw(column_width) << std::setprecision(precision)
         << properties[i].kn
         << std::setw(column_width) << std::setprecision(precision)
         << properties[i].ks;
      // Normal and tanget forces.
    column_width = 8;
    file << std::setw(column_width) << std::setprecision(precision)
         << normal_forces[i]
         << std::setw(column_width) << std::setprecision(precision)
         << tangent_forces[i];
    column_width = 11;
      // Forces.
    file<< std::setw(column_width) << std::setprecision(precision)
         << forces[i].x_component
         << std::setw(column_width) << std::setprecision(precision)
         << forces[i].y_component
      // Accelerations.
         << std::setw(column_width) << std::setprecision(precision)
         << accelerations[i].x_component
         << std::setw(column_width) << std::setprecision(precision)
         << accelerations[i].y_component
      // Velocities.
         << std::setw(column_width) << std::setprecision(precision)
         << velocities[i].x_component
         << std::setw(column_width) << std::setprecision(precision)
         << velocities[i].y_component
      // Displacements.
         << std::setw(column_width) << std::setprecision(precision)
         << displacements[i].x_component
         << std::setw(column_width) << std::setprecision(precision)
         << displacements[i].y_component << "\n";
  }

  // Write contacts data.
  file << "\nCONTACTS\n"
       << std::setw(column_width) << "p1_idx"
       << std::setw(column_width) << "p2_idx"
       << std::setw(column_width) << "overlap\n";
  for (size_t i = 0; i < contacts_size; ++i) {
    file << std::setw(column_width) << contacts_buffer[i].p1_idx
         << std::setw(column_width) << contacts_buffer[i].p2_idx
         << std::setw(column_width) << std::setprecision(precision)
         << contacts_buffer[i].overlap << "\n";
  }
}
