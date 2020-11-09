#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include "data.h"
#include "initialization.h"
#include "config.h"

int main(){
    int N = 5;
    int * p = (int*)calloc(N, sizeof(int));
    int * pp[]= {&p[0], &p[1], &p[2], &p[3], &p[4]};            //(int**)calloc(N, sizeof(int*));
    for(int i=0; i<N;++i){
        //pp[i] = &p[i];
        p[i] = i;
    }
    for(int i=0; i<N;++i)
        printf("%d: padd:%x p:%d pp:%x\n", i,  &p[i], p[i], pp[i]);
}
