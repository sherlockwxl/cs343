//
// Created by xilingwu on 2/15/19.
//
#include "q2matrixmultiply.h"


#define type MIMPL
matrixMultiplier::matrixMultiplier(int *Z[], int *X[], unsigned int xr, unsigned int xc, int *Y[], unsigned int yc):
    Z(Z), X(X), xr(xr), xcyr(xc), Y(Y), yc(yc){}

#if type == TASK

#elif type == CFOR
/*
 * create concurrency using cofor
 */
void matrixMultiplier::main(){
    int totalcount = xr * yc;

    COFOR( i, 0, totalcount,

    int row = i/xr; //get the row number from i
    int col = i%xr; //get the col number from i
    // for ( int i = 0; i < totalcount; r += 1 )
    Z[row][col] = 0;

    for ( int index = 0; index < xc; index += 1 )
        Z[row][col] += X[row][index] * Y[index][col];
    ); // wait for threads

    //multiply done
}





#elif type == ACTOR

#endif

