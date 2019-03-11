#include "q1printer.h"
#include <iostream>
using namespace std;

// class constructor
Printer::Printer( unsigned int voters ){
    voters = voters;
    buffer = deque<Printer::data>(voters);
    for(unsigned int i = 0 ; i < voters; i++)
    {
        cout << "V" << i;
        if( i != voters - 1){
            cout << "\t";
        }
    }

    cout << "\n";

    for(unsigned int i = 0 ; i < voters; i++)
    {
        cout << "*******";
        if( i != voters - 1){
            cout << "\t";
        }
    }

    cout << "\n";

}

// class destructor will print the end message
Printer::~Printer(){
    cout << "*****************" <<endl;
    cout << "All tours started" <<endl;
}

// flush function

void Printer::flush(){
    for(unsigned int i = 0 ; i < voters; i++){
        if(buffer[i].set){

            buffer[i].set = false;                //reset the flag
            cout << (char)buffer[i].state;
            switch(buffer[i].state){
                case Voter::States::Vote:
                    cout << " " << buffer[i].ballot.picture << "," << buffer[i].ballot.statue<<
                    ","<< buffer[i].ballot.giftshop;
                    break;
                case Voter::States::Block:
                case Voter::States::Unblock:
                    cout << " " << buffer[i].numBlocked;
                    break;
                case Voter::States::Finished:
                    cout << " "<< buffer[i].tour.tourkind << " " << buffer[i].tour.groupno;
                default:
                    break;

            }// end switch
            cout << "\t";
        }

        cout << "\n";
    }
}

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