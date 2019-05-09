#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
extern "C" {
  #include "config.h"
}

using namespace std;

/**
 * Parses the provided config file,
 * and stores the results in the provided structure.
 */
void parse_config(const char *filename, Config *config) {
  ifstream config_file;
  config_file.open(filename, std::ifstream::in);

  string line;
  string key;
  string value;
  while (getline(config_file, line)) {
    // Skip empty lines.
    if (line.empty()) {
      continue;
    }

    // Parse each line.
    istringstream is_line(line);
    if (getline(is_line, key, '=')) {
      if (getline(is_line, value)) {
        if (key == "n") {
          sscanf(value.c_str(), "%zu", &config->simulation_size);
        } else if (key == "time") {
          config->simulation_time = stoi(value);
        } else if (key == "dt") {
          config->dt = stod(value);
        } else if (key == "kn") {
          config->kn = stod(value);
        } else if (key == "ks") {
          config->ks = stod(value);
        } else if (key == "r") {
          config->r = stod(value);
        } else if (key == "m") {
          config->m = stod(value);
        } else if (key == "v0") {
          config->v0 = stod(value);
        } else {
          cerr << "Invalid key: " << key << endl;
        }
      } else {
        cerr << "Missing value for property: " << key << endl;
      }
    } else {
      cerr << "Missing '=' in: " << line << endl;
    }
  }
}
