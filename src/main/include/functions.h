#pragma once

#include "data.h"

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
int size_triangular_matrix(const int n);

/**
 * Takes two arrays of vectors,
 * and sums together the vectors with the same index.
 */
void sum_vectors(const int size, const Vector *v1, const Vector *v2, Vector *result);

/**
 * Applies the forces to the particles with the same index,
 * and computes the resultant acceleration.
 */
void compute_acceleration(const int size, const ParticleProperties *particles, const Vector *forces, Vector resultant_accelerations);

/**
 * Derives the resultant velocity,
 * of an initial velocity with an applied acceleration for given a time delta.
 */
void compute_velocity(const int size, const Vector *accelerations, const double dt, Vector velocities);

/**
 * Computes the displacement of the particles,
 * with an applied velocity for a given time delta.
 */
void compute_displacement(const int size, const Vector *velocities, const double dt, Particle *particles);
