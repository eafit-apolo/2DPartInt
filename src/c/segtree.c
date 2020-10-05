#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "segtree.h" 




void print_particle(struct Particle p){
    printf("X: %f Y: %f\n", p.x, p.y);
}


void print_node(struct Node n){
    if(n.p.empty){
        printf(" %f  || ", n.p.y);
    }else{
        printf(" NONE  || ");
    }
}


int getMid(int s, int e){
    return s + (e - s) / 2;
}


void insert(struct Node T[], int ai, struct Particle p, int k){
    struct Node v = T[k];

    if (v.kl != v.kr){

        int mid = getMid(v.kl, v.kr);
        if(p.y <= mid){
            insert(T, ai*2 + 1, p, k);
        }else{
            insert(T, ai*2 + 2, p, k);
        }

    }else{
        v.p = p;
    }

}


void initialize(struct Node st[], int l, int r, int i){
    struct Node n = {l, r};
    st[i] = n;
    if (l == r){
        return;
    }
    int mid = getMid(l, r);
    initialize(st, l, mid, i*2 + 1);
    initialize(st, mid + 1, r, i*2 + 2);
}


int main(int argc, char *argv[]){
    int n = 10;
    int x = ceil(log2(n));
    int max_size = 2 * (pow(2, x)) - 1;
    struct Node st[max_size];
    int ys[] = {0, 2, 3, 1, 5, 4};
    return 0;
}
