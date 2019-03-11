#include "q1voter.h"
#include "q1printer.h"

extern MPRNG mprng;

// class constructor for voter
Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ):
    id(id), nvotes(nvotes), voteTallier(voteTallier), printer(printer){};

// main function
void Voter::main(){

    // yield 0 - 19 first
    yield(mprng(19));

    //print start message
    for(unsigned int i = 0 ; i < nvotes; i++){
        printer.print(id, States::Start);


        //second yield
        yield(mprng(4));

        //random a vote
        TallyVotes::Ballot vote = cast();

        TallyVotes::Tour voteres;

        try{
            _Enable{
                    voteres  = voteTallier.vote(id, vote);
            }
        } catch (TallyVotes::Failed){
            yield(mprng(4));
            printer.print(id, Voter::States::Failed);
            break;
        }

        yield(mprng(4));
        printer.print(id, States::Finished, voteres);

    }
    voteTallier.done();
    printer.print(id, States::Terminated);

}


