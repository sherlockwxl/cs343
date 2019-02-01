#include "binsertsort.h"
#include <iostream>
#include <fstream>
using namespace std;
template<typename T>

void Binsertsort<T>::main(){
    T pivot;
    Binsertsort<T> left(Sentinel);
    Binsertsort<T> right(Sentinel);
    pivot = value;
    suspend();
    for(;;){
        if(value == Sentinel){
            break;//
        }
        if(value <= pivot){
            left.sort(value);
        }else{
            right.sort(value);
        }

        suspend();
    }

    suspend();

    for(;;){
        for(;;){
            
        }
    }


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
        Binsertsort<TYPE> binsertsort(Sentinel);
        unsigned int elementcount;
        currentline >> elementcount;//get current line element count

        TYPE currentelement;

        for(unsigned int i = 0; i < elementcount; i++){
            currentline >> currentelement;
            *outfile << currentelement << " ";
            binsertsort.sort(currentelement);
        }

        *outfile << endl;

        binsertsort.sort(Sentinel)

        for(unsigned int i = 0 ; i < elementcount; i++){
            *outfile << binsertsort.retrive() << " ";
        }

        *outfile << endl;
    } // for
    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
} // main
