#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


struct Particle
{
    int x;
    int y;
    bool empty;
};


struct Node
{
    int kl;
    int kr;
    struct Particle p;
    struct Node * penultimate;
    struct Node * parent;
};


void print_particle(struct Particle p){
    printf("X: %d Y: %d\n", p.x, p.y);
}


void print_node(struct Node n){
    if(n.p.empty){
        printf(" %d  || ", n.p.y);
    }else{
        printf(" NONE  || \n");
    }
}

void str_node(char* s, struct Node n){
    if(n.p.empty){
        sprintf(s, " %d  || ", n.p.y);
    }else{
        sprintf(s, " NONE  || ");
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
    size_t leny = sizeof(ys) / sizeof(int);
    //
    struct Particle arr[leny];
    for(int i = 0; i < leny; ++i){
        struct Particle a = {i, ys[i], false};
        arr[i] = a;
    }
    //
    for(int i = 0; i < leny; ++i){
        //print_particle(arr[i]);
    }

    /* UNTIL HERE IS WORKING FINE */

    // initialize segment tree
    initialize(st, 0, n - 1, 0);
    size_t lenst = sizeof(st) / sizeof(int);
    for(int i = 0; i < lenst; ++i){
        print_node(st[i]);
    }
    // print empty tree
    int st_i = 0;
    for(int i = 1; i <= pow(2, x); i=pow(2, i)){
        char s[200];
        for(int j = 0; j < i; ++j){
            int l = st[st_i].kl;
            int r = st[st_i].kr;
            char s2[20];
            sprintf(s2, "(%d:%d)", l, r);
            strcat(s, s2);
            //str_node(s, st[st_i]);
            st_i++;
        }
        //printf("%s\n", s);
    }
    //
    return 0;
}
