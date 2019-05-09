#pragma once

/**
 * Represents the parsed config file.
 */
typedef struct {
  size_t simulation_size;
  int simulation_time;
  double dt;
  double kn;
  double ks;
  double r;
  double m;
  double v0;
} Config;

/**
 * Parses the provided config file,
 * and stores the results in the provided structure.
 */
void parse_config(const char *config_file, Config *config);
