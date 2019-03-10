#include "q1tallyVotes.h"
#include "q1voter.h"
using namespace std;

// tally voter implementation using  mutex/condition solution
TallyVotes:TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    voters(voters), group(group), printer(printer){ groupnumber = 0; voted = 0; voterLeft = voters;};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){

    ownerLock.acquire();                   //acquire owner lock

    if()
    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        bargingLock.wait(ownerLock);
        if(bargingLock.empty()){
            bargingFlag = false;
        }
    }

    voted++;                               // update the current voted number
    printer.print(id, Voter::States::Vote, Ballot ballot)
}

void TallyVotes::done(){

}