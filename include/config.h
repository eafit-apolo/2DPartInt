#pragma once

/**
 * Represents the parsed config file.
 */
typedef struct {
  int simulation_time;
  double dt;
  int x_limit;
  int y_limit;
  int radius;
  double kn;
  double ks;
  double rho;
  double thickness;
  double v0;
  double r0;
} Config;

/**
 * Parses the provided config file,
 * and stores the results in the provided structure.
 */
void parse_config(const char *filename, Config *config);
