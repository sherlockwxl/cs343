#ifndef q2matrix
#define q2matrix

#define CFOR 0
#define TASK 1
#define ACTOR 2

#ifndef MIMPL
#define MIMPL  CFOR
#endif

using namespace std;

#if MIMPL == TASK
/*
 * create concurrency using task
 */
_Task matrixMultiplier{

    int **Z;            //result matrix
    int **X;            //input matrix x
    int **Y;            //input matrix y
    unsigned int xr;    //row number of x
    unsigned int xcyr;  //col number of x/ row number of y
    unsigned int yc;    //col number of y
    int lowerbound;     //current row number range in Z, and used to calculate nex row number for two sub tasks.
    int upperbound;
    void main();

public:
    matrixMultiplier(int *Z[], int *X[], int *Y[], unsigned int xr, unsigned int xc, unsigned int yc,
        int lowerbound, int upperbound);


};

#elif MIMPL == CFOR
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

#elif MIMPL == ACTOR
#include <uActor.h>
//customize message struct
struct multiplierMsg:public uActor::Message{
    int *Z;        //only access one row of Z in each actor
    int *X;        //only access one row of X in each actor
    int **Y;       //only access one col of Y in each actor
    int xcyr;      //col number of xc/xcyr
    int yc;        //col number of Y
    multiplierMsg(int *Z, int *X, int *Y[], int xcyr, int yc) : Message(uActor::Delete),
    Z(Z), X(X), Y(Y), xcyr(xcyr), yc(yc){};

};
/*
 * create concurrency using actor
 */
_Actor matrixMultiplier{

    int *Z;           //result matrix
    int *X;           //input matrix x
    int **Y;          //input matrix y
    int xcyr;      //col number of xc/xcyr
    int yc;        //col number of Y

    Allocation receive(Message & msg){
        Case( multiplierMsg, msg){
            Z = msg_d->Z;
            X = msg_d->X;
            Y = msg_d->Y;
            xcyr = msg_d->xcyr;
            yc = msg_d->yc;
            for(int i = 0; i < xcyr; i++){
                for(int j = 0 ; j < yc; j++){
                    Z[j] += X[i] * Y[i][j];
                }
            }
        } else Case( StopMsg, msg){

        }
        return Delete;
    }



public:
    //matrixMultiplier(){};

};


#endif

#endif
