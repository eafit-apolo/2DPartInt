#pragma once

#include "data.h"

/**
 * Computes the euclidean distance between two particles.
 */
double compute_distance(const Particle *p1, const Particle *p2);

/**
 * Computes the overlap between two particles.
 */
double compute_overlap(const Particle *p1, const Particle *p2);

/**
 * Applies gravity to a particle.
 */
void apply_gravity(const ParticleProperties *particles_properties,
                   const size_t particle_index, Vector *forces);

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
size_t size_triangular_matrix(const size_t n);

/**
 * Computes the forces applied to each particle.
 */
void compute_forces(const double dt, const size_t particles_size, const size_t particle_index,
                    const size_t contacts_size, const Particle *particles,
                    const ParticleProperties *properties, const Contact *contacts,
                    const Vector *velocities, double *normal_forces,
                    double *tangent_forces, Vector *forces);

/**
 * Applies the forces to the particles with the same index,
 * and computes the resultant acceleration.
 */
void compute_acceleration(const size_t particle_index,
                          const ParticleProperties *particles_properties,
                          const Vector *forces, Vector *accelerations);

/**
 * Derives the resultant velocity,
 * of an initial velocity with an applied acceleration for given a time delta.
 */
void compute_velocity(const double dt, const size_t particle_index,
                      const Vector *accelerations, Vector *velocities);

/**
 * Computes the displacement of the particles,
 * with an applied velocity for a given time delta.
 */
void compute_displacement(const double dt, const size_t particle_index,
                          const Vector *velocities, Vector *displacements);

/**
 * Displace all particles given their displacements.
 */
void displace_particle(const size_t particle_index, const Vector *displacements,
                       Particle *particles);

/**
 * Changes the displacement if the new position would surpass the X or Y limit.
 */
void fix_displacement(const size_t particle_index, Vector *velocities, Particle *particles);

/**
 * Computes the contacts between one particle and the particles with greater index.
 * Returns the number of contacts written on the buffer.
 */
size_t compute_contacts(const size_t size, const Particle *particles, const size_t particle_index,
                        Contact *contacts_buffer);
