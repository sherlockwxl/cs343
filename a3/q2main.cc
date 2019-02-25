//
// Created by xilingwu on 2/15/19.
//
#include <iomanip>
#include "q2matrixmultiply.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdio>

//

using namespace std;
bool printmode = false;

//helper function for allocate space for x,y,z matrix
void createMatrix(int *matrix[], int rows, int cols){
    for(int i = 0; i < rows; i++){
        matrix[i] = new int[cols];
        for(int j = 0 ; j < cols; j++){
            matrix[i][j] = 0;
        }
    }
}

//helper function for load the file/37 into the corresponding matrix
void loadMatrix(int *matrix[], int rows, int cols, istream* matrixData){
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(printmode == true){
                *matrixData >> matrix[i][j];
            }else{
                matrix[i][j] = 37;
            }
        }
    }
}

//helper function to print the output
void printresult(int *Xmatrix[], int *Ymatrix[], int *Zmatrix[], int xr, int xcyr, int yc){
    //first print space and matrix Y
    for(int i = 0 ; i < xcyr; i++){
        //print space
        for(int j = 0 ; j < xcyr; j++){
            cout<<"        ";
        }
        cout<<"   | ";
        //print matrix Y
        for(int z = 0; z < yc; z++){
            cout<<setw(8)<<Ymatrix[i][z];
        }
        cout<<endl;
    }

    //print the splitter line

    for(int i = 0 ; i < xcyr * 8 + 3; i++){
        cout<<"-";
    }

    cout<<"*";
    for(int i = 0 ; i < yc * 8 + 1; i++){
        cout<<"-";
    }
    cout<<endl;

    //print the bot half: X and Z
    for(int i = 0 ; i < xr; i++){
        //print i row in X
        for(int j = 0 ; j < xcyr; j++){
            cout<<setw(8)<<Xmatrix[i][j];
        }
        cout<<"   | ";
        //print matrix Z
        for(int z = 0; z < yc; z++){
            cout<<setw(8)<<Zmatrix[i][z];
        }
        cout<<endl;
    }
}

//interface for 3 different implementations, the interface will ini the task/actor/class
void matrixmultiply( int *Z[], int *X[], unsigned int xr, unsigned int xc, int *Y[], unsigned int yc ){

#if defined(TASK)
    matrixMultiplier M(Z, X, Y, xr, xc, yc, 0, xr - 1);
#endif

#if defined(CFOR)
    matrixMultiplier M(Z, X, Y, xr, xc, yc);
#endif


#if defined(ACTOR)

    uActorStart();    // call uactor start first
    for(unsigned int i = 0; i < xr; i++){
        *(new matrixMultiplier())| *new multiplierMsg(Z[i], X[i], Y, xc, yc);
    }
    uActor::stop();   // wait for all actor to stop
#endif
}




int main( int argc, char * argv[] ) {


    //set up default and ini variables
    istream * xmatrix;
    istream * ymatrix;
    int processors = 1;
    int xr;
    int xcyr;
    int yc;

    try {                                               // process command-line arguments
        if(argc < 4){
            throw 1;
        }
        switch ( argc ) {
            case 6:
                printmode = true;
                try {                   // open input file
                    ymatrix = new ifstream( argv[5] );   // open input yfile

                } catch( uFile::Failure & ) {
                    cerr << "Error! Cannot open y-matrix input-file \"" << argv[5]  << "\"" << endl;
                    throw 1;
                } // try

                try {                   // open input file
                    xmatrix = new ifstream( argv[4] );   // open input xfile

                } catch( uFile::Failure & ) {
                    cerr << "Error! Cannot open x-matrix input-file \"" << argv[4] << "\"" << endl;
                    throw 1;
                } // try

            case 5:
                if (argc != 6){
                    processors = stoi( argv[4] ); if ( processors <= 0 ) throw 1;
                }


            case 4:
                yc = stoi( argv[3] ); if ( yc < 1) throw 1;

            case 3:
                xcyr = stoi( argv[2] ); if ( xcyr < 1) throw 1;

            case 2:
                xr = stoi( argv[1] ); if ( xr < 1) throw 1;
            case 1: break;                                // use all defaults
            default: throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0] << " xrows (> 0) xycols (> 0) ycols (> 0)"
                                        "[ processors (> 0) | x-matrix-file  y-matrix-file ]"<< endl;
        exit( EXIT_FAILURE );
    } // try

    uProcessor p[processors - 1];  // number of kernel threads

    //now we have three variables initialized, we could create the three matrixs

    int *X[xr];                     // declare and initialize X, Y, and Z matrix
    int *Y[xcyr];
    int *Z[xr];

    //create space for x,y,z matrix;
    createMatrix(X, xr, xcyr);
    createMatrix(Y, xcyr, yc);
    createMatrix(Z, xr, yc);

    //cout<<"create done"<<endl;
    //now we load the given file/37 into the x and y matirx;
    loadMatrix(X, xr, xcyr, xmatrix);
    loadMatrix(Y, xcyr, yc, ymatrix);


    //cout<<"load done"<<endl;
    //do the matrix multiply
    matrixmultiply(Z, X, xr, xcyr, Y, yc);


    //cout<<"cal done"<<endl;
    //if x/y file is provided print the result
    if(printmode){
        printresult(X, Y, Z, xr, xcyr, yc);
    }


    //now free space of X, Y, Z
    for(int i = 0 ; i < xr; i++){
        delete X[i];
        delete Z[i];
    }
    for(int i = 0 ; i < xcyr; i++){
        delete Y[i];
    }

    if(printmode == true){
        delete xmatrix;
        delete ymatrix;
    }

}
