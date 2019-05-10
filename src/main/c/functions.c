#include <math.h>
#include <stdlib.h>
#include <string.h> // For memset.
#include "data.h"
#include "functions.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
size_t size_triangular_matrix(const size_t n) {
  return ((n - 1) * n) / 2;
}

/**
 * Computes the distance between two particles.
 */
double compute_distance(const Particle *p1, const Particle *p2) {
  const double x_diff = p1->x_coordinate - p2->x_coordinate;
  const double y_diff = p1->y_coordinate - p2->y_coordinate;

  return sqrt((x_diff * x_diff) + (y_diff * y_diff));
}

/**
 * Computes the overlap between two particles.
 * Note: If the overlap is negative, there is no overlap.
 */
double compute_overlap(const Particle *p1, const Particle *p2) {
  const double d = p1->radious + p2->radious;
  const double distance = compute_distance(p1, p2);

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
 * Apply gravity to a vector of forces.
 */
void apply_gravity(const size_t size, const ParticleProperties *particles, Vector *forces) {
  for (size_t i = 0; i < size; ++i) {
    forces[i].y_component -= (particles[i].mass * 9.81d);
  }
}

/**
 * Compute the forces applied to P2 given it was collided by P1.
 * Note: previous_normal and previous_tangent correspond to P2 with respect to P1.
 */
void collide_two_particles(const double dt, const Particle *p1, const Particle *p2, const Vector *velocity_p1, const Vector *velocity_p2, const ParticleProperties *properties_p2, Vector *force_p2, double *previous_normal, double *previous_tangent) {
  const double distance = compute_distance(p1, p2);

  const Vector normal = {
    .x_component = (p2->x_coordinate - p1->x_coordinate) / distance,
    .y_component = (p2->y_coordinate - p1->y_coordinate) / distance
  };

  const double normal_velocity = (normal.x_component * (velocity_p1->x_component - velocity_p2->x_component))
      + (normal.y_component * (velocity_p1->y_component - velocity_p2->y_component));

  const double tangent_velocity = (normal.y_component * (velocity_p1->x_component - velocity_p2->x_component))
      - (normal.x_component * (velocity_p1->y_component - velocity_p2->y_component));

  const double dfn = normal_velocity * properties_p2->kn * dt;
  const double dfs = tangent_velocity * properties_p2->ks * dt;

  double Fn_1_2 = *previous_normal + dfn;
  double Fs_1_2 = *previous_tangent + dfs;

  if (Fn_1_2 < 0) {
    Fn_1_2 = 0;
    Fs_1_2 = 0;
  }

  const double Fs_1_2_max = Fn_1_2 * tan((30 * M_PI) / 180);

  if (abs(Fs_1_2) > Fs_1_2_max) {
    Fs_1_2 = (abs(Fs_1_2_max) * abs(Fs_1_2)) / Fs_1_2;
  }

  // Update the forces of p2.
  force_p2->x_component += (-normal.x_component * Fn_1_2) - (normal.y_component * Fs_1_2);
  force_p2->y_component += (-normal.y_component * Fn_1_2) + (normal.x_component * Fs_1_2);

  // Update the normal and tangent forces between p1 and p2 for the next simulation step.
  *previous_normal = Fn_1_2;
  *previous_tangent = Fs_1_2;
}

void compute_forces(const double dt, const size_t particles_size, const size_t contacts_size,
                    const Particle *particles, const ParticleProperties *properties, const Contact *contacts,
                    const Vector *velocities, double *normal_forces, double *tangent_forces, Vector *forces) {
  memset(forces, 0, sizeof(Vector) * particles_size);

  size_t p1_idx, p2_idx;
  for(size_t i = 0; i < contacts_size; ++i) {
    p1_idx = contacts[i].p1_idx;
    p2_idx = contacts[i].p2_idx;

    // P1 collides P2.
    collide_two_particles(
      dt,
      &particles[p1_idx],
      &particles[p2_idx],
      &velocities[p1_idx],
      &velocities[p2_idx],
      &properties[p2_idx],
      &forces[p2_idx],
      &normal_forces[(p1_idx * particles_size) + p2_idx],
      &tangent_forces[(p1_idx * particles_size) + p2_idx]
    );

    // P2 collides P1.
    collide_two_particles(
      dt,
      &particles[p2_idx],
      &particles[p1_idx],
      &velocities[p2_idx],
      &velocities[p1_idx],
      &properties[p1_idx],
      &forces[p1_idx],
      &normal_forces[(p2_idx * particles_size) + p1_idx],
      &tangent_forces[(p2_idx * particles_size) + p1_idx]
    );
  }

  apply_gravity(particles_size, properties, forces);
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
  Particle p1, p2;
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i + 1; j < size; ++j) {
      p1 = particles[i];
      p2 = particles[j];
      overlap = compute_overlap(&p1, &p2);
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
