#pragma once

#include "data.h"

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
size_t size_triangular_matrix(const size_t n);

/**
 * Takes two arrays of vectors,
 * and sums together the vectors with the same index.
 */
void sum_vectors(const size_t size, const Vector *v1, const Vector *v2, Vector *result);

/**
 * Computes the forces applied to each particle.
 */
void compute_forces(const double dt, const size_t particles_size, const size_t contacts_size,
                    const Particle *particles, const ParticleProperties *properties, const Contact *contacts,
                    const Vector *velocities, double *normal_forces, double *tangent_forces, Vector *forces);

/**
 * Applies the forces to the particles with the same index,
 * and computes the resultant acceleration.
 */
void compute_acceleration(const size_t size, const ParticleProperties *particles, const Vector *forces, Vector *resultant_accelerations);

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
 * Computes the contacts between all particles.
 * Returns the number of contacts written on the buffer.
 * Note: The size of the buffer should be equals to
 *       the size of a triangular matrix for the number of particles.
 */
size_t compute_contacts(const size_t size, const Particle *particles, Contact *contacts_buffer);
