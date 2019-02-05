#include "q2binsertsort.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
/*
 * Example of non-basic type, the new type is named dictionary and it contains a key and a char;
 */
struct dictionary{
    int key;
    char c;



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

ostream& operator<<(ostream& os, const dictionary& d){
    os << " key: " << d.key << " value : " << d.c << " ";

    return os;
}


//End of non-basic type example


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


    Binsertsort<T> less;
    Binsertsort<T> greater;

    try {
        _Enable{
                for (;;) {
                    if (value < pivot) {
                        less.sort(value);
                    } else {
                        greater.sort(value);
                    }
                    suspend(); //wait for next input
                }
        }

    }catch(Sentinel & s){//get event and end insert stage

        _Resume Binsertsort<TYPE>::Sentinel() _At less;//pass the event to left routine

        _Resume Binsertsort<TYPE>::Sentinel() _At greater;//pass the event to right routine

    }


    //retrieve stage

    //retrieve from left routine
    try{
        _Enable{
                for(;;){
                    if(less.set == 0){//when left was not initialized end current loop
                        break;
                    }else{

                        value = less.retrieve();//retrieve left node value
                        suspend();
                    }


                }
        }
    }
    _CatchResume( Sentinel&) {
        _Throw Sentinel();

    }
    catch (Sentinel& ) {
        //do nothing
    }
    //retrieve left done

    //retrieve current node
    value = pivot;
    try {
        _Enable{
                suspend();
        }
    }_CatchResume( Sentinel&) {
        _Throw Sentinel();
    }
    catch ( Sentinel& ) {
        //do nothing
    }

    //retrieve from right routine
    try{
        _Enable{
                for(;;){
                    if(greater.set == 0){//when right was not initialized end current loop
                        break;
                    }else{
                        value = greater.retrieve();//retrieve right node value
                        suspend();
                    }


                }
        }
    }
    _CatchResume( Sentinel& ) {
        _Throw Sentinel();
    }
    catch ( Sentinel& ) {
        //do nothing
    }


    //all routine finished => end
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

            binsertsort.sort(currentelement);//insert each of input value.
        }

        *outfile << endl;

        _Resume Binsertsort<TYPE>::Sentinel() _At binsertsort;//raise sentinel event at root coroutine

        for(unsigned int i = 0 ; i < elementcount; i++){
            if(i > 0){
                *outfile<< " ";
            }
            *outfile <<  binsertsort.retrieve();//call retrieve method on root coroutine
        }

        *outfile << endl <<endl;
    } // for
    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    if  (outfile != &cout) delete outfile;
} // main
