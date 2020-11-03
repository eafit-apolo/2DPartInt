#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"
#include "collisions.h"

/**
 * Given a point (x, y), find the index for the square that should contain it.
 */
int find_square(const int x, const int y, const int x_squares, const int y_squares, const double square_length){
    int row_ind = find_row(y,y_squares,square_length);
    int col_ind = find_col(x,x_squares,square_length);
    if(col_ind < 0 || row_ind < 0){
        return -1;
    }
    return row_ind * x_squares + col_ind;
}

int find_col(const int x, const int x_squares, const double square_length){
    double x_left_limit = -(x_squares*square_length/2); // The far leftest (if that word exists) of the grid
    double diff = x_left_limit -x;
    if(diff>0) return -2; // outside the left boundary
    int col_ind = floor(fabs(diff)/square_length);
    if(col_ind>=x_squares) return -1; // outisde the right boundary
    return col_ind;
}

int find_row(const int y, const int y_squares, const double square_length){
    int row_ind = floor(((double)y)/square_length);
    if(row_ind >= y_squares) return -1;
    if(row_ind < 0) return -2;
    return row_ind;
}

size_t compute_contacts(Particle** grid, const int x_squares, const int y_squares, const double square_length, Contact* contacts){
    size_t k = 0; // current number of contacts
    for(size_t row=0; row<y_squares; row++){
        for(size_t col=0; col<x_squares; col++){
            size_t square_idx = row*x_squares+col;
            //printf("\n");
            if(!grid[square_idx]) continue;
            Particle* first = grid[square_idx];
            Particle* p = grid[square_idx];
            while(p){
                // First compare with particles within the same square
                Particle* other = first;
                while(other){
                    if(other != p){
                        if(!other){
                            //printf("\n\nother is null in SAME SQUARE\n\n");
                        }
                        if(!p){
                            //printf("\n\np is null in SAME SQUARE\n\n");
                        }
                        //printf("in SAME SQUARE  - p: %d , radius: %f - other: %d , radius: %f\n", p->idx, p->radius,  other->idx, other->radius);
                        const double overlap = compute_overlap(p, other);
                        //printf("OVERLAP: %f\n", overlap);
                        if(overlap > 0){
                            //printf("INSIDE > )\n");
                            contacts[k].p1_idx = p->idx;
                            contacts[k].p2_idx = other->idx;
                            contacts[k].overlap = overlap;
                            k++;
                        }
                    }
                    other = other->next;
                }
                int left_col = find_col(p->x_coordinate - p->radius*2, x_squares, square_length);
                if(left_col == -2) left_col = 0;
                int right_col = find_col(p->x_coordinate + p->radius*2, x_squares, square_length);
                if(right_col == -1) right_col = x_squares-1;
                int bottom_row = find_row(p->y_coordinate - p->radius*2, y_squares, square_length);
                if(bottom_row==-2) bottom_row = 0;
                int top_row = find_row(p->y_coordinate + p->radius*2, y_squares, square_length);
                if(top_row==-1) top_row=y_squares-1;

                for(int neighbor_row=bottom_row; neighbor_row<=top_row; neighbor_row++){
                    for(int neighbor_col=left_col; neighbor_col<=right_col;neighbor_col++){
                        size_t neighbor_square_idx = neighbor_row*x_squares+neighbor_col;
                        other = grid[neighbor_square_idx];
                        //printf("\nCOLLGRID: %ld\n", neighbor_square_idx);
                        if(other==NULL) continue;
                        if(neighbor_square_idx==square_idx) continue;
                        while(other){
                            if(other==NULL){
                                //printf("\n\nother is null in NEIGHBORS\n\n");
                            }
                            if(p==NULL){
                                //printf("\n\np is null in NEIGHBORS\n\n");
                            }
                            //printf("in NEIGHBORS  - p: %d , radius: %f - other: %d , radius: %f\n", p->idx, p->radius,  other->idx, other->radius);
                            const double overlap = compute_overlap(p, other);
                            if(overlap > 0){
                                contacts[k].p1_idx = p->idx;
                                contacts[k].p2_idx = other->idx;
                                contacts[k].overlap = overlap;
                                k++;
                            }
                            other = other->next;
                            //printf("\nPOINTER: %p", (void*)other);
                        }
                    }
                }
                p = p->next;
            }
        }
    }
    return k;
}

/**
 * Fills the grid with particles. The side length of each square grid is twice the diameter of each particle,
 * since all particles have the same radius. Running time is O(N), and memory space is O(N) but really 2*N because of 'lasts' pointers array.
 */
void fill_grid(const int num_particles, Particle* particles, Particle** grid, const int x_squares, const int y_squares,
        const double square_length){
    double x_left_limit = -(x_squares*square_length/2); // The far leftest (if that word exists) of the grid
    Particle** lasts = (Particle**) calloc(x_squares * y_squares, sizeof(Particle*)); // Array of pointers to last positions

    // For each particle, find its square. Keep in mind that grid is an array of  pointers to the first element in the squares, and that
    // lasts is an array of pointers to the last element in the squares.
    int counter = 0;
    for(int i=0; i<num_particles; i++){
        Particle* p = &particles[i];
        p->next=NULL; // Make sure we do not do anything funny
        int square_ind = find_square(p->x_coordinate, p->y_coordinate, x_squares, y_squares, square_length);
        if(square_ind< 0) continue; // The particle is outside the boundaries of our grid
        counter++;
        //printf("FILLGRID:%d\n", square_ind);

        if(!lasts[square_ind]){ // if is the first element in the square
            grid[square_ind] = p;
            lasts[square_ind] = p;
        }else{
            lasts[square_ind]->next=p;
            lasts[square_ind] = p;
        }
    }

    free(lasts);
}


int print_grid(Particle** grid, const int x_squares, const int y_squares, int iter_num){
    int total_particles = 0;
    printf("STEP: %d\n", iter_num);
    for(int i=0; i < x_squares*y_squares;i++){
        if(!grid[i]) continue;
        Particle* p = grid[i];
        int counter = 1;
        while(p->next){
            p = p->next;
            counter++;
        }
        total_particles+=counter;
        if(counter!=0)
        printf("G%d: %d , ", i, counter);
    }
    printf("\nTOTAL: %d\n\n", total_particles);
    return total_particles;
}
