//
// Created by xilingwu on 2/15/19.
//
#include <iostream>
#include <uCobegin.h>
#include "q2matrixmultiply.h"




using namespace std;

matrixMultiplier::matrixMultiplier(int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc):
        Z(Z), X(X),  Y(Y), xr(xr), xcyr(xc), yc(yc){calculate();}




#if MIMPL == TASK

#elif MIMPL == CFOR
/*
 * create concurrency using cofor
 */
void matrixMultiplier::calculate(){
    cout <<" main called "<< MIMPL <<endl;
    int totalcount = xr * yc;

    COFOR( i, 0, totalcount,

    int row = i/xr; //get the row number from i
    int col = i%xr; //get the col number from i
    // for ( int i = 0; i < totalcount; r += 1 )
    Z[row][col] = 0;

    for ( int index = 0; index < xcyr; index += 1 )
        Z[row][col] += X[row][index] * Y[index][col];
    ); // wait for threads

    //multiply done
}





#elif MIMPL == ACTOR
#endif


