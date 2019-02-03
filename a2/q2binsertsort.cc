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
    cout<<" new tree start curent pivot "<< pivot << endl;
    try{
        _Enable{
                suspend();
        };

    }catch(Sentinel & s){//when received input done at level 1

        cout<<" received input done  in waitng level 1 current pivot : "<<pivot << endl;
        insertdone = 1;
        value = pivot;

        suspend();
        _Resume Sentinel() _At resumer();
    }


    Binsertsort<T> left;
    Binsertsort<T> right;
    if(insertdone != 1){

        try {
            _Enable{
                    for (;;) {
                        cout << " get next input " << value << " current " << pivot<<endl;
                        if (value <= pivot) {
                            cout << " pass to left " << endl;
                            left.sort(value);
                        } else {
                            cout << " pass to right " << endl;
                            right.sort(value);
                        }

                        cout << " wait next input at current " << pivot<<endl;
                        suspend();


                    }
            }

        }catch(Sentinel & s){
            cout<<" received input done in waitng level 2 current pivot : "<< pivot << endl;
            _Resume Binsertsort<TYPE>::Sentinel() _At left;
            _Resume Binsertsort<TYPE>::Sentinel() _At right;
            insertdone = 1;

        }
    }


    //retrive
    cout<<" start to retrive at "<<pivot<<endl;
    try{
        _Enable{
                for(;;){
                    if(left.set == 0){
                        cout<<" break left retrive at "<<pivot<<endl;
                        break;
                    }else{
                        cout<<" call left retrive at "<<pivot<<endl;
                        value = left.retrieve();
                        cout<<"left retrive return "<<value<<endl;
                        suspend();
                    }


                }
        }
    }
    _CatchResume( Sentinel& ) {
        _Throw Sentinel();
    }
    catch ( Sentinel& ) {

    }


    value = pivot;
    cout<<" node return  "<<pivot<<endl;
    try {
        _Enable{
                suspend();
        }
    }catch ( Sentinel& ) {
        // end of less and terminate child node
    }

    try{
        _Enable{
                for(;;){
                    if(right.set == 0){
                        cout<<" break right retrive at "<<pivot<<endl;
                        break;
                    }else{
                        cout<<" call right retrive at "<<pivot<<endl;
                        value = right.retrieve();
                        cout<<"right retrive return "<<value<<endl;
                        suspend();
                    }


                }
        }
    }
    _CatchResume( Sentinel& ) {
        _Throw Sentinel();
    }
    catch ( Sentinel& ) {

    }

   // _Resume Sentinel() _At resumer();

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
            *outfile << currentelement << " ";
            binsertsort.sort(currentelement);
        }

        *outfile << endl;

        cout<<"Done input"<<endl<<endl;
        _Resume Binsertsort<TYPE>::Sentinel() _At binsertsort;

        for(unsigned int i = 0 ; i < elementcount; i++){
            cout<<"Main call retrive:  ";
            int value = binsertsort.retrieve();
            *outfile << value << " ";
            cout<<" and get :  " << value <<endl;
        }

        *outfile << endl;
    } // for
    if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    if  (outfile != &cout) delete outfile;
} // main
