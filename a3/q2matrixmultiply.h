#ifndef q2matrix
#define q2matrix

using namespace std;
#define type MIMPL
/*
#if type == TASK
/*
 * create concurrency using task

_Task matrixMultiplier{

    int **Z;           //result matrix
    int **X;           //input matrix x
    int **Y;           //input matrix y
    unsigned int xr;    //row number of x
    unsigned int xcyr;  //col number of x/ row number of y
    unsigned int yc;    //col number of y

    void main();

public:
    matrixMultiplier( int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc);


};
*/
//#elif type == CFOR
/*
 * create concurrency using cofor
 */
class matrixMultiplier{

    int **Z;           //result matrix
    int **X;           //input matrix x
    int **Y;           //input matrix y
    unsigned int xr;    //row number of x
    unsigned int xcyr;  //col number of x/ row number of y
    unsigned int yc;    //col number of y

    //void main();
    void calculate();

public:
    matrixMultiplier( int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc);

};
/*
#elif type == ACTOR
/*
 * create concurrency using actor

_Actor matrixMultiplier{

    int **Z;           //result matrix
    int **X;           //input matrix x
    int **Y;           //input matrix y
    unsigned int xr;    //row number of x
    unsigned int xcyr;  //col number of x/ row number of y
    unsigned int yc;    //col number of y

    void main();

public:
    matrixMultiplier( int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc);

};


#endif
*/
#endif
