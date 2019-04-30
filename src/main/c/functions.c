#include <math.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
int size_triangular_matrix(const int n) {
  return ((n - 1) * n) / 2;
}

/**
 * Computes the overlap between two particles.
 * Note: If the overlap is negative, there is no overlap.
 */
double compute_overlap(const Particle p1, const Particle p2) {
  double d = p1.radious + p2.radious;

  double x_diff = p1.x_coordinate - p2.x_coordinate;
  double y_diff = p1.y_coordinate - p2.y_coordinate;
  double distance = sqrt((x_diff * x_diff) + (y_diff * y_diff));

  return d - distance;
}

/**
 * Takes two arrays of vectors, and sums together the vectors with the same index.
 */
void sum_vectors(const int size, const Vector *v1, const Vector *v2, Vector *result) {
  for (int i = 0; i < size; ++i) {
    result[i].x_component = v1[i].x_component + v2[i].x_component;
    result[i].y_component = v1[i].y_component + v2[i].y_component;
  }
}

void compute_acceleration(const int size, const ParticleProperties *particles, const Vector *forces, Vector *resultant_accelerations){
  for (int i = 0; i < size; ++i) {
    resultant_accelerations[i].x_component = forces[i].x_component / particles[i].mass;
    resultant_accelerations[i].y_component = forces[i].y_component / particles[i].mass;
  }
}

/**

* Derives the resultant velocity,
 * of an initial velocity with an applied acceleration for given a time delta.
 */
void compute_velocity(const int size, const Vector *accelerations, const double dt, Vector *velocities){
  for (int i = 0; i < size; ++i) {
    velocities[i].x_component = velocities[i].x_component + accelerations[i].x_component * dt;
    velocities[i].y_component = velocities[i].y_component + accelerations[i].y_component * dt;
  }
}

/**
 * Computes the contacts between all particles.
 * Returns the number of contacts written on the buffer.
 * Note: The size of the buffer should be equals to
 *       the size of a triangular matrix for the number of particles.
 */
int compute_contacts(const int size, const Particle *particles, Contact *contacts_buffer) {
  int k = 0;
  double overlap;
  Particle p1;
  Particle p2;
  for (int i = 0; i < size; ++i) {
    for (int j = i + 1; j < size; ++j) {
      p1 = particles[i];
      p2 = particles[j];
      overlap = compute_overlap(p1, p2);
      if (overlap > 0) {
        contacts_buffer[k].p1_idx = i;
        contacts_buffer[k].p2_idx = j;
        contacts_buffer[k].overlap = overlap;
        ++k;
      }
    }
  }
  return k;
}
