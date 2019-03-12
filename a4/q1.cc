#include <iostream>
#include "q1printer.h"
#include "q1voter.h"
#include "q1tallyVotes.h"

#include "MPRNG.h"

using namespace std;
MPRNG mprng;


// define output variable
#ifdef OUTPUT
    bool printmode = true;
#else
    bool printmode = false;
#endif

int main( int argc, char * argv[] ) {


    //set up default and ini variables
    int processors = 1;
    int seed = getpid();
    int votes = 1;
    int group = 3;
    int voters = 6;

    try {                                               // process command-line arguments
        switch ( argc ) {
            case 6: if ( strcmp( argv[5], "d" ) != 0 ) {  // default ?
                    processors = stoi( argv[5] ); if ( processors <= 0 ) throw 1;
                } //if
            case 5: if ( strcmp( argv[4], "d" ) != 0 ) {  // default ?
                    seed = stoi( argv[4] ); if ( seed <= 0 ) throw 1;
                } //if
            case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
                    votes = stoi( argv[3] ); if ( votes <= 0) throw 1;
                } // if
            case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
                    group = stoi( argv[2] ); if ( group <= 0 ) throw 1;
                } // if
            case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                    voters = stoi( argv[1] ); if ( voters < 0 ) throw 1;
                } // if
            case 1: break;                                // use all defaults
            default: throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0] << " [ voters (> 0) | 'd' (default 6) [ group (> 0) | 'd' (default 3) "
                                        "[ votes (> 0) | 'd' (default 1) [ seed (> 0), | 'd' (default random) "
                                        "[ processors (> 0), | 'd' (default 1) ] ] ] ] ]"
        << endl;
        exit( EXIT_FAILURE );
    } // try

    mprng.set_seed(seed);  //set up seed
    uProcessor p[processors - 1]; // number of kernel threads


    // create printer
    Printer printer(voters);
    // create tallyvoter
    TallyVotes tv(voters, group, printer);
    // create list of voters;
    Voter * voterlist[voters];

    // create each voter
    for( int i = 0 ; i < voters; i++){
        
        voterlist[i] = new Voter(i, votes, tv, printer);
    }

    // all done remove voter;

    for( int i = 0 ; i < voters; i++){
        delete voterlist[i];
    }




}



