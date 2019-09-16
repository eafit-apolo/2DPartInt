#pragma once

/**
 * Represents the parsed config file.
 */
typedef struct {
  int simulation_time;
  double dt;
  int x_limit; // Size in X.
  int y_limit; // Size in Y.
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
