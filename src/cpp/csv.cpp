#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
extern "C" {
  #include "data.h"
}
#include "csv.h"

/**
 * Ensures the output folder exists
 * Note: If not, it tries to create it.
 */
int ensure_output_folder(const char *output_folder) {
  return system(("mkdir -p " + std::string(output_folder)).c_str());
}

/**
 * Writes a CSV file that can be read by ParaView,
 * with the current status of the simulation.
 * The file will be written on the specified folder,
 * and suffixed with the step number.
 */
void write_simulation_step(const size_t num_particles, const Particle *particles,
                           const char *folder, const unsigned long step) {
  // Open the csv file to write.
  std::ofstream output_file;
  output_file.open(
    std::string(folder) + "/2DPartInt-Out.bin." + std::to_string(step),
    std::ios::out | std::ios::binary
  );

  // Write the header.
  output_file << "x coord, y coord, z coord, radius\n";

  for (size_t i = 0; i < num_particles; ++i) {
    //char* coordinate = particles[i].x_coordinate' + '-' + particles[i].y_coordinate
    //+ '-' + '0' + '-' + particles[i].radius + '\n';
    output_file.write((char*)&particles[i].x_coordinate, sizeof(particles[i].x_coordinate));
    output_file.write((char*)&", ", sizeof(", "));
    output_file.write((char*)&particles[i].y_coordinate, sizeof(particles[i].y_coordinate));
    output_file.write((char*)&", ", sizeof(", "));
    output_file.write((char*)&"0", sizeof("0"));
    output_file.write((char*)&", ", sizeof(", "));
    output_file.write((char*)&particles[i].radius, sizeof(particles[i].radius));
    output_file.write((char*)&"\n", sizeof("\n"));
  }
  // Write the current status of each particle.
/*  for (size_t i = 0; i < num_particles; ++i) {
    output_file << particles[i].x_coordinate
                << ", "
                << particles[i].y_coordinate
                << ", "
                << 0 // Z Coordinate.
                << ", "
                << particles[i].radius
                << "\n";
  }*/

  // Close the CSV file.
  output_file.close();
}
