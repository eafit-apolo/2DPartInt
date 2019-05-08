#pragma once

#include "data.h"

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
size_t size_triangular_matrix(const size_t n);

/**
 * Computes the overlap between two particles.
 * Note: If the overlap is negative, there is no overlap.
 */
double compute_overlap(const Particle p1, const Particle p2);

/**
 * Takes two arrays of vectors,
 * and sums together the vectors with the same index.
 */
void sum_vectors(const size_t size, const Vector *v1, const Vector *v2, Vector *result);

/**
 * Applies the forces to the particles with the same index,
 * and computes the resultant acceleration.
 */
void compute_acceleration(const size_t size, const ParticleProperties *particles, const Vector *forces, Vector *accelerations);

/**
 * Derives the resultant velocity,
 * of an initial velocity with an applied acceleration for given a time delta.
 */
void compute_velocity(const size_t size, const Vector *accelerations, const double dt, Vector *velocities);

/**
 * Computes the displacement of the particles,
 * with an applied velocity for a given time delta.
 */
void compute_displacement(const size_t size, const double dt, const Vector *velocities, Vector *displacements);

/**
 * Displace all particles given their displacements.
 */
void displace_particles(const size_t size, const Vector *displacements, Particle *particles);

/**
 * Computes the contacts between all particles.
 * Returns the number of contacts written on the buffer.
 * Note: The size of the buffer should be equals to
 *       the size of a triangular matrix for the number of particles.
 */
size_t compute_contacts(const size_t size, const Particle *particles, Contact *contacts_buffer);
