//
// Created by xilingwu on 2/15/19.
//
#include "q2matrixmultiply.h"

//interface for 3 different implementations, the interface will ini the task/actor/class
void matrixmultiply( int *Z[], int *X[], unsigned int xr, unsigned int xc, int *Y[], unsigned int yc ){
    matrixMultiplier M(Z, X, xr, xc, Y, yc);
}




void 
