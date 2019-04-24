#include "data.h"
#include "functions.h"

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
