#include "q2binsertsort.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
template<typename T>

void Binsertsort<T>::main(){
    T pivot;
    pivot = value;
    set = 1;
    try{
        suspend();
    }catch(Sentinel & s){
        //_Throw left::Sentinel();
        //_Throw right::Sentinel();
        _Throw Sentinel();
        insertdone = 1;
    }

    Binsertsort<T> left();
    Binsertsort<T> right();

    for(;;){
        if(value <= pivot){
            left.sort(value);
        }else{
            right.sort(value);
        }

        try{
            suspend();
        }catch(Sentinel & s){
            //_Throw left.Sentinel();
            //_Throw right.Sentinel();
            _Throw Sentinel();
            insertdone = 1;
            break;
        }
    }

    for(;;){
        if(left.set == 0){
            _Throw Sentinel();
        }
        value = left.retrieve();
        try{
            suspend();
        }catch(Sentinel & s){
            break;
        }
    }

    value = pivot;
    suspend();

    for(;;){
        if(right.set == 0){
            _Throw Sentinel();
        }
        value = right.retrieve();
        try{
            suspend();
        }catch(Sentinel &){
            break;
        }
    }

    _Throw Sentinel();





}

int main( int argc, char * argv[] ) {
    istream * infile = &cin;				// default value
    ostream * outfile = &cout;				// default value

    try {
	switch ( argc ) {
	  
	  case 3:
	    try{
	    	outfile = new ofstream( argv[2] );
	    }catch( uFile::Failure & ) {		// open failed ?
			cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
			throw 1;
		} // try
	  case 2:
          try {                   // open input file
              infile = new ifstream( argv[1] );   // open input file
              break;
          } catch( uFile::Failure & ) {
              cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
              throw 1;
          } // try
	  case 1:					// defaults
	    break;
	  default:					// wrong number of options
	    throw 1;
	} // switch
    } catch( ... ) {
	cerr << "Usage: " << argv[0]
	     << "unsorted-file [ sorted-file ]" << endl;
	exit( EXIT_FAILURE );				// TERMINATE
    } // try
    for ( ;; ) {                    // copy input file to output file
        string line;
        getline(*infile, line);
        if(infile -> fail()) break;  //EOF -> exit;
        if(line.empty()){
            cout << "\'\' : Warning! Blank line." <<endl;
            continue;
        }

        stringstream currentline(line);

        Binsertsort<TYPE> binsertsort;
        unsigned int elementcount;
        currentline >> elementcount;//get current line element count

        TYPE currentelement;

        for(unsigned int i = 0; i < elementcount; i++){
            currentline >> currentelement;
            *outfile << currentelement << " ";
            binsertsort.sort(currentelement);
        }

        *outfile << endl;

        _Throw Sentinel();

        for(unsigned int i = 0 ; i < elementcount; i++){
            *outfile << binsertsort.retrieve() << " ";
        }

        *outfile << endl;
    } // for
    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
} // main
