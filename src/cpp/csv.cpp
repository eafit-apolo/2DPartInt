#include <cstdlib>
#include <fstream>
#include <string>
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
    std::string(folder) + "/2DPartInt-Out.csv." + std::to_string(step),
    std::ios_base::out | std::ios_base::trunc
  );

  // Write the header.
  output_file << "x coord, y coord, z coord, radius\n";

  // Write the current status of each particle.
  for (size_t i = 0; i < num_particles; ++i) {
    output_file << particles[i].x_coordinate
                << ", "
                << particles[i].y_coordinate
                << ", "
                << 0 // Z Coordinate.
                << ", "
                << particles[i].radius
                << "\n";
  }

  // Close the CSV file.
  output_file.close();
}

void write_grid(const int x_squares, const int y_squares, const double square_length, const char* folder)
{
    // Open the csv file to write.
    std::ofstream output_file;
    output_file.open(
        std::string(folder) + "/2DPartInt-Out-GRID.csv",
        std::ios_base::out | std::ios_base::trunc);

    // Write the header.
    output_file << "x coord, y coord, length\n";

    double x_left_limit = -(x_squares*square_length/2); // The far leftest (if that word exists) of the grid
    double y=0;
    for (int row = 0; row < y_squares; ++row) {
        double x = x_left_limit;
        for (int col = 0; col < x_squares; ++col) {
            output_file << x
                        << ", "
                        << y
                        << ", "
                        << square_length // Z Coordinate.
                        << "\n";
            x+=square_length;
        }
        y+=square_length;
    }

    // Close the CSV file.
    output_file.close();
}

void write_particles_from_grid(const int x_squares, const int y_squares, const char* folder, Particle** grid, const int step)
{
    // Open the csv file to write.
    std::ofstream output_file;
    output_file.open(
        std::string(folder) + "/2DPartInt-Out-FROM-GRID.csv." + std::to_string(step),
        std::ios_base::out | std::ios_base::trunc);

    // Write the header.
    output_file << "x coord, y coord, length\n";
    int counter = 0;
    for (int row = 0; row < y_squares; ++row) {
        for (int col = 0; col < x_squares; ++col) {
            if(!grid[row*x_squares + col]) continue; // If  square is empty
            Particle* p = grid[row * x_squares + col];
            do{
                counter++;
                output_file << p->x_coordinate
                            << ", "
                            << p->y_coordinate
                            << ", "
                            << 0
                            << ", "
                            << p->radius
                            << "\n";
                p = p->next;
            }while(p);
        }
    }
}
