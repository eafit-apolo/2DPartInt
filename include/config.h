#pragma once

/**
 * Represents the parsed config file.
 */
typedef struct {
  int simulation_time;
  double dt;
  int m; // Size in X.
  int n; // Size in Y.
  double kn;
  double ks;
  double mass;
  double v0;
} Config;

/**
 * Parses the provided config file,
 * and stores the results in the provided structure.
 */
void parse_config(const char *filename, Config *config);
