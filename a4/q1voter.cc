#include "q1voter.h"
#include "q1printer.h"
#include <iostream>
extern MPRNG mprng;
extern bool printmode;
// class constructor for voter
Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ):
    id(id), nvotes(nvotes), voteTallier(voteTallier), printer(printer){};

// main function
void Voter::main(){

    // yield 0 - 19 first
    yield(mprng(19));

    //print start message
    for(unsigned int i = 0 ; i < nvotes; i++){
        if(printmode)
        printer.print(id, States::Start);


        //second yield
        yield(mprng(4));

        //random a vote
        TallyVotes::Ballot vote = cast();

        TallyVotes::Tour voteres;

        try{

            voteres  = voteTallier.vote(id, vote);
            yield(mprng(4));
            if(printmode)
                printer.print(id, States::Finished, voteres);
        } catch (TallyVotes::Failed &){

            if(printmode)
            printer.print(id, Voter::States::Failed);
            break;
        }



    }

    voteTallier.done();
    if(printmode)
    printer.print(id, States::Terminated);





}


