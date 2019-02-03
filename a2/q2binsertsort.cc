#include "q2binsertsort.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct dictionary{
    int key;
    char c;

    dictionary(int key, char c){
        key = key;
        c = c;
    }

};

bool operator==(const dictionary& d1, const dictionary&d2){
    if(d1.c == d2.c && d1.key == d2.key){
        return true;
    }
    return false;
}

bool operator < (const dictionary& d1, const dictionary&d2){
    if(d1.key < d2.key){
        return true;
    }
    return false;
}

istream& operator>>(istream& is, dictionary& d){
    is>>d.key;
    d.c = '0' + d.key;
    return is;
}

ostream& operator<<(ostream& os, dictionary& d){
    os << " key: " << d.key << " value : " << d.c << " ";

    return os;
}

template<typename T>


void Binsertsort<T>::main(){
    T pivot;
    pivot = value;
    set = 1;
    try{
        _Enable{
                suspend();
        };

    }catch(Sentinel & s){//when received input done at level 1

        value = pivot;
        suspend();//pass value back

        _Resume Sentinel() _At resumer();//end current routine
    }


    Binsertsort<T> left;
    Binsertsort<T> right;

    try {
        _Enable{
                for (;;) {
                    if (value <= pivot) {
                        left.sort(value);
                    } else {
                        right.sort(value);
                    }
                    suspend(); //wait for next input
                }
        }

    }catch(Sentinel & s){//get event and end insert stage

        _Resume Binsertsort<TYPE>::Sentinel() _At left;//pass the event to left routine

        _Resume Binsertsort<TYPE>::Sentinel() _At right;//pass the event to right routine

    }


    //retrive

    try{
        _Enable{
                for(;;){
                    if(left.set == 0){//when left was not initialized end current loop
                        break;
                    }else{

                        value = left.retrieve();//retrive left node
                        suspend();
                    }


                }
        }
    }
    _CatchResume( Sentinel& s) {
        _Throw Sentinel();

    }
    catch (Sentinel& s ) {
        //do nothing
    }

    //retirve current node
    value = pivot;
    try {
        _Enable{
                suspend();
        }
    }_CatchResume( Sentinel& s) {
        _Throw Sentinel();
    }
    catch ( Sentinel& s ) {
        //do nothing
    }

    try{
        _Enable{
                for(;;){
                    if(right.set == 0){
                        break;
                    }else{
                        value = right.retrieve();
                        suspend();
                    }


                }
        }
    }
    _CatchResume( Sentinel& ) {
        _Throw Sentinel();
    }
    catch ( Sentinel& s) {
        //do nothing
    }

    _Resume Sentinel() _At resumer();
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
            if(i > 0){
                *outfile<< " ";
            }
            *outfile << currentelement;

            binsertsort.sort(currentelement);
        }

        *outfile << endl;

        _Resume Binsertsort<TYPE>::Sentinel() _At binsertsort;
        for(unsigned int i = 0 ; i < elementcount; i++){
            if(i > 0){
                *outfile<< " ";
            }
            *outfile <<  binsertsort.retrieve();
        }

        *outfile << endl <<endl;
    } // for
    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    if  (outfile != &cout) delete outfile;
} // main
