#include <math.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
size_t size_triangular_matrix(const size_t n) {
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
void sum_vectors(const size_t size, const Vector *v1, const Vector *v2, Vector *result) {
  for (size_t i = 0; i < size; ++i) {
    result[i].x_component = v1[i].x_component + v2[i].x_component;
    result[i].y_component = v1[i].y_component + v2[i].y_component;
  }
}

/**
 * Applies the forces to the particles with the same index,
 * and computes the resultant acceleration.
 */
void compute_acceleration(const size_t size, const ParticleProperties *particles, const Vector *forces, Vector *resultant_accelerations){
  for (size_t i = 0; i < size; ++i) {
    resultant_accelerations[i].x_component = forces[i].x_component / particles[i].mass;
    resultant_accelerations[i].y_component = forces[i].y_component / particles[i].mass;
  }
}

/**
 * Computes the displacement of the particles,
 * with an applied velocity for a given time delta.
 */
void compute_displacement(const size_t size, const double dt, const Vector *velocities, Vector *displacements){
  for (size_t i = 0; i < size; ++i) {
    displacements[i].x_component = displacements[i].x_component + velocities[i].x_component * dt;
    displacements[i].y_component = displacements[i].y_component + velocities[i].y_component * dt;
  }
}

/** Derives the resultant velocity,
 * of an initial velocity with an applied acceleration for given a time delta.
 */
void compute_velocity(const size_t size, const Vector *accelerations, const double dt, Vector *velocities){
  for (size_t i = 0; i < size; ++i) {
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
size_t compute_contacts(const size_t size, const Particle *particles, Contact *contacts_buffer) {
  size_t k = 0;
  double overlap;
  Particle p1;
  Particle p2;
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i + 1; j < size; ++j) {
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