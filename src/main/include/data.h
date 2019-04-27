#pragma once

/**
 * Represents a circular shaped particle in a two-dimensional space.
 */
typedef struct {
  double x;
  double y;
  double radious;
} Particle;

/**
 * Physical properties of a particle,
 * used in the forces and acceleration computations.
 */
typedef struct {
  double mass;
} ParticleProperties;

/**
 * Represents a vector in a two-dimensional space.
 */
typedef struct {
  double x_component;
  double y_component;
} Vector;

/**
 * Represents a contact between two particles.
 */
typedef struct {
  int p1_idx;
  int p2_idx;
  unsigned double overlap;
} Contact;
