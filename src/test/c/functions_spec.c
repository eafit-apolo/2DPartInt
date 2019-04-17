#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

// Maximum acceptable error when comparing double values.
#define TOLERANCE 0.05d

// Terminal color constants.
#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESETCOLOR "\033[0m"

// Global var to keep track of the number of failed tests.
int number_failed;

/**
 * Asserts that two double values are equals, within the allowed tolerance.
 * Prints to console if the test passed or not.
 */
void assert(const double result, const double expected, const char* test_description) {
  if (fabs(result - expected) > TOLERANCE) {
    number_failed++;
    printf(RED "Test: '%s' FAILED!\n\t%f did not equal %f\n" RESETCOLOR, test_description, expected, result);
  } else {
    printf(GREEN "Test: '%s' PASSED!\n" RESETCOLOR, test_description);
  }
}

/**
 * Asserts that two double values are equals, within the allowed tolerance.
 * Prints to console if the test passed or not, together with the iteration number.
 */
void for_assert(const double result, const double expected, const char* test_description, const int i) {
  if (fabs(result - expected) > TOLERANCE) {
    number_failed++;
    printf(RED "Test: '%s' (%i) FAILED!\n\t%f did not equal %f\n" RESETCOLOR, test_description, i, expected, result);
  } else {
    printf(GREEN "Test: '%s' (%i) PASSED!\n" RESETCOLOR, test_description, i);
  }
}

/**
 * Checks that the sum_vectors function works for arrays of one element.
 */
void test_sum_vectors_one_element() {
  #define size 1
  Vector v1[size] = { { 0, 0 } };
  Vector v2[size] = { { 10, 10 } };
  Vector result[size] = { { 0 } };

  sum_vectors(size, v1, v2, result);

  assert(result[0].x_component, 10.0d, "test_sum_vectors_one_element - x_component");
  assert(result[0].y_component, 10.0d, "test_sum_vectors_one_element - y_component");
  #undef size
}

/**
 * Checks that the sum_vectors function works for arrays of multiple elements.
 */
void test_sum_vectors_multiple_elements() {
  #define size 3
  Vector v1[size] = { { 0, 0 }, { -1.5, 9.99 }, { 3.0, 5.0 } };
  Vector v2[size] = { { 10, 10 }, { -11.11, -5.43 }, { -5.0, 3.0 } };
  Vector result[size] = { { 0 } };

  sum_vectors(size, v1, v2, result);

  Vector expected[size] = { { 10.0d, 10.0d }, { -12.6d, 4.56d }, { -2.0d, 8.0d } };
  for (int i = 0; i < size; i++) {
    for_assert(result[i].x_component, expected[i].x_component, "test_sum_vectors_multiple_elements - x_component", i);
    for_assert(result[i].y_component, expected[i].y_component, "test_sum_vectors_multiple_elements - y_component", i);
  }
  #undef size
}

/**
 * Tests entry point.
 * All tests run here.
 */
int main(void) {
  // Initialize the number of failed tests.
  number_failed = 0;

  // Execute all tests.
  test_sum_vectors_one_element();
  test_sum_vectors_multiple_elements();

  // If, at least one test failed, exit with an error code.
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
