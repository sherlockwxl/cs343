#include "q2printer.h"
#include <iostream>
using namespace std;
extern bool printmode;

// class constructor
Printer::Printer( unsigned int voters ){

    this->voters = voters;
    this->buffer = deque<Printer::data>(voters);   // use queue to store data for easy access by index

    // print start info when constructor is called

    for(unsigned int i = 0 ; i < voters; i++)
    {
        if(printmode)
        cout << "V" << i;
        if( i != voters - 1){
            if(printmode)
            cout << "\t";
        }
    }

    if(printmode)
    cout << endl;

    for(unsigned int i = 0 ; i < voters; i++)
    {
        if(printmode)
        cout << "*******";
        if( i != voters - 1){
            if(printmode)
            cout << "\t";
        }
    }
    if(printmode)
    cout << endl;

}

// class destructor will print the end message
Printer::~Printer(){

    if(printmode){
        flush();
        cout << "*****************" <<endl;
        cout << "All tours started" <<endl;
    }

}

// flush function

void Printer::flush(){

    //get last index to print \t
    int last = -1;
    for( int i = voters - 1; i >= 0 ; i--){
        if(last == -1 && buffer[i].set){
            last = i;
        }
    }


    for(unsigned int i = 0 ; i < voters; i++){
        if(buffer[i].set){

            buffer[i].set = false;                //reset the flag
            cout << (char)buffer[i].state;

            switch(buffer[i].state){              // for case not listed only state info is required to print
                case Voter::States::Vote:
                    cout << " " << buffer[i].ballot.picture << "," << buffer[i].ballot.statue<<
                    ","<< buffer[i].ballot.giftshop;
                    break;
                case Voter::States::Block:
                case Voter::States::Unblock:
                    cout << " " << buffer[i].numBlocked;
                    break;
                case Voter::States::Finished:
                    cout << " "<< (char)buffer[i].tour.tourkind << " " << buffer[i].tour.groupno;
                default:
                    break;

            }// end switch
            if( (int)i < last){
                cout << "\t";
            }
        }else{
            if( (int)i < last){
                cout << "\t";
            }
        }


    }

    cout <<  endl;
}

// function is called to determine if flush is required, if do will call flush and set current state
void Printer::checkflush(unsigned int id){
    if(buffer[id].set){
        flush();
    }
    buffer[id].set = true;
}

// print function for update the state
void Printer::print( unsigned int id, Voter::States state ){
    checkflush(id);
    buffer[id].state = state;
}

// print function for update the state and tour
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ){
    checkflush(id);
    buffer[id].state = state;
    buffer[id].tour = tour;
}

// print function for update the state and ballot
void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ){
    checkflush(id);
    buffer[id].state = state;
    buffer[id].ballot = ballot;
}

// print function for update the state and numBlocked
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ){
    checkflush(id);
    buffer[id].state = state;
    buffer[id].numBlocked = numBlocked;
}