#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "config.h"

/**
 * Parses the provided config file,
 * and stores the results in the provided structure.
 */
void parse_config(const char *filename, Config *config) {
  std::ifstream config_file;
  config_file.open(filename, std::ios_base::in);

  std::string line;
  std::string key;
  std::string value;
  while (std::getline(config_file, line)) {
    // Skip empty lines.
    if (line.empty()) {
      continue;
    }

    // Parse each line.
    std::istringstream is_line(line);
    if (std::getline(is_line, key, '=')) {
      if (std::getline(is_line, value)) {
        if (key == "time") {
          config->simulation_time = std::stod(value);
        } else if (key == "dt") {
          config->dt = std::stod(value);
        } else if (key == "y_particles") {
          config->y_particles = std::stoi(value);
        } else if (key == "x_particles") {
          config->x_particles = std::stoi(value);
        } else if (key == "y_squares") {
          config->y_squares = std::stoi(value);
        } else if (key == "x_squares") {
          config->x_squares = std::stoi(value);
        } else if (key == "square_in_grid_length") {
          config->square_in_grid_length = std::stod(value);
        } else if (key == "radius") {
          config->radius = std::stof(value);
        } else if (key == "kn") {
          config->kn = std::stod(value);
        } else if (key == "ks") {
          config->ks = std::stod(value);
        } else if (key == "rho") {
          config->rho = std::stod(value);
        } else if (key == "thickness") {
          config->thickness = std::stod(value);
        } else if (key == "v0") {
          config->v0 = std::stod(value);
        } else if (key == "r0") {
          config->r0 = std::stod(value);
        } else {
          std::cerr << "Invalid key: " << key << std::endl;
        }
      } else {
        std::cerr << "Missing value for property: " << key << std::endl;
      }
    } else {
      std::cerr << "Missing '=' in: " << line << std::endl;
    }
  }

  config_file.close();
}
