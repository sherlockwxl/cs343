//
// Created by xilingwu on 2/15/19.
//
#include <iostream>
#include <uCobegin.h>
#include "q2matrixmultiply.h"




using namespace std;






#if MIMPL == TASK
matrixMultiplier::matrixMultiplier(int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc,
        int lowerbound, int upperbound):
        Z(Z), X(X),  Y(Y), xr(xr), xcyr(xc), yc(yc), lowerbound(lowerbound), upperbound(upperbound){}

void matrixMultiplier::main(){
    if(lowerbound > upperbound){
        return;
    }else{
        int currentrow = (lowerbound + upperbound)/2;
        //matrixMultiplier* lowerPartTask = new matrixMultiplier( Z, X, Y, xr, xc, yc, lowerbound, currentrow);
        //matrixMultiplier* upperPartTask = new matrixMultiplier( Z, X, Y, xr, xc, yc, currentrow + 1, upperbound);
        matrixMultiplier matrixMultiplier1( Z, X, Y, xr, xcyr, yc, lowerbound, currentrow - 1);
        matrixMultiplier matrixMultiplier2( Z, X, Y, xr, xcyr, yc, currentrow + 1, upperbound);
        //calculate current row value;

        //cout << "calcualteing " << currentrow<<endl;
        for( int i = 0 ; i < (int)yc; i++){
            for( int j = 0 ; j < (int)xcyr; j++){
                Z[currentrow][i] += X[currentrow][j] * Y[j][i];
            }
        }



    }
}

#elif MIMPL == CFOR
matrixMultiplier::matrixMultiplier(int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc):
        Z(Z), X(X),  Y(Y), xr(xr), xcyr(xc), yc(yc){calculate();}

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
    //Z[row][col] = 0;

    for ( int index = 0; index < xcyr; index += 1 )
        Z[row][col] += X[row][index] * Y[index][col];
    ); // wait for threads

    //multiply done
}





#elif MIMPL == ACTOR


#endif


