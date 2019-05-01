#include <math.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

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
 * Computes the forces applied to each particle.
 */
void compute_forces(const size_t size, const size_t contacts_size, const Particle *particles,
        const ParticleProperties *properties, const Vector *velocities, double normal_forces[size][size],
        double tangent_forces[size][size], const double dt, const Contact *contacts, Vector *resultant_forces){
    
    double distance, normal_vector_x, normal_vector_y, Fs_1_2max, normal_velocity, tangent_velocity, dfn, dfs, Fn_1_2, Fs_1_2;
    size_t p1_idx, p2_idx;
    for(size_t i = 0;i<contacts_size;++i){
        p1_idx = contacts[i].p1_idx;
        p2_idx = contacts[i].p2_idx;
        
        distance = sqrt(pow((particles[p1_idx].x_coordinate - particles[p2_idx].x_coordinate),2) +
                pow((particles[p1_idx].y_coordinate - particles[p2_idx].y_coordinate), 2));
        normal_vector_x = (particles[p1_idx].x_coordinate - particles[p2_idx].x_coordinate)/distance;
        normal_vector_y = (particles[p1_idx].y_coordinate - particles[p2_idx].y_coordinate)/distance;

        normal_velocity = normal_vector_x * (velocities[p1_idx].x_component - velocities[p2_idx].x_component) +
            normal_vector_y * (velocities[p1_idx].y_component - velocities[p2_idx].y_component);
        tangent_velocity =  normal_vector_y * (velocities[p1_idx].x_component - velocities[p2_idx].x_component) -
            normal_vector_x * (velocities[p1_idx].y_component - velocities[p2_idx].y_component);

        dfn = properties->kn * normal_velocity * dt;
        dfs = properties->ks * tangent_velocity * dt;
          

        Fn_1_2 = normal_forces[p1_idx][p2_idx] + dfn;
        Fs_1_2 = tangent_forces[p1_idx][p2_idx] + dfs;

        if( Fn_1_2 < 0 ){
            Fn_1_2 = 0;
            Fs_1_2 = 0;
        }

        Fs_1_2max = Fn_1_2 * tan(30*M_PI / 180);
        
        
        if( abs(Fs_1_2) > Fs_1_2max){
            Fs_1_2 = abs(Fs_1_2max) * abs(Fs_1_2)/Fs_1_2;
        }

        normal_forces[p1_idx][p2_idx] = Fn_1_2;
        tangent_forces[p1_idx][p2_idx] = Fs_1_2;
    
        resultant_forces[p2_idx].x_component = -normal_vector_x * Fn_1_2 -
            normal_vector_y * Fs_1_2;
        resultant_forces[p2_idx].y_component = -normal_vector_y * Fn_1_2 +
            normal_vector_x * Fs_1_2;
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
