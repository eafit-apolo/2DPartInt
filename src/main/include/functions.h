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
 * Applies the forces to the particles with the same index,
 * and computes the resultant acceleration.
 */
void compute_acceleration(const size_t size, const ParticleProperties *particles, const Vector *forces, Vector *accelerations);

/**
 * Derives the resultant velocity,
 * of an initial velocity with an applied acceleration for given a time delta.
 */
void compute_velocity(const double dt, const size_t size, const Vector *accelerations, Vector *velocities);

/**
 * Computes the displacement of the particles,
 * with an applied velocity for a given time delta.
 */
void compute_displacement(const double dt, const size_t size, const Vector *velocities, Vector *displacements);

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
