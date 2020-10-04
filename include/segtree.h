 
#pragma once

struct Particle
{
    bool empty;
    double x;
    double y;
} Particle_default = {true, 0, 0};


struct Node
{
    struct Particle p;
    int kl;
    int kr;
    struct Node * penultimate;
    struct Node * parent;
};

