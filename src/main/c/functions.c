#include "data.h"
#include "functions.h"

/**
 * Returns the size of a triangular matrix, without the diagonal.
 */
int size_triangular_matrix(const int n) {
  return ((n - 1) * n) / 2;
}

/**
 * Takes two arrays of vectors, and sums together the vectors with the same index.
 */
void sum_vectors(const int size, const Vector *v1, const Vector *v2, Vector *result) {
  for (int i = 0; i < size; i++) {
    result[i].x_component = v1[i].x_component + v2[i].x_component;
    result[i].y_component = v1[i].y_component + v2[i].y_component;
  }
}
