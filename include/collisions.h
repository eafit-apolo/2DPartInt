#pragma once

#include "data.h"


int find_col(const int x, const int x_squares, const double square_length);
int find_row(const int y, const int y_squares, const double square_length);
int find_square(const int x, const int y, const int x_squares, const int y_squares, const double square_length);

void fill_grid(const int num_particles, Particle* particles, Particle** grid, const int x_squares, const int y_squares,
        const double square_length);

int print_grid(Particle** grid, const int x_squares, const int y_squares, int iter_num);
size_t compute_contacts(Particle** grid, const int x_squares, const int y_squares, const double square_length, Contact* contacts);
