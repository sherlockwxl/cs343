#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"
#include <iostream>
#include <algorithm>
#include <uSemaphore.h>
using namespace std;

extern bool printmode;
// helper function to reset the counter
void TallyVotes::resetcount(){
    currentBallot.statue = 0;
    currentBallot.picture = 0;
    currentBallot.giftshop = 0;
}

// helper function to release the sem
void TallyVotes::resetlock(){
    bargingFlag = false;
    if(!waitForGroupSem.empty())
        waitForGroupSem.V();
    if(!bargingSem.empty())
        bargingSem.V();
}


// tally voter implementation using  semaphore solution
// default value set 1 for mutex sem and 0 for sync sem
TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
        ownerSem(1), waitForGroupSem(0), bargingSem(0),voters(voters), group(group), printer(printer){
        groupnumber = 1;
        voted = 0;
        voterLeft = voters;
        resetcount();
};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){

    ownerSem.P();                  //acquire owner sem


    if(voterLeft < group){                 // when need to
        resetlock();
        ownerSem.V();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        if(printmode)
        printer.print(id, Voter::States::Barging); // print the barging message

        bargingSem.P(ownerSem);

        if(voterLeft < group){                 // may have issue with no catch flow
            resetlock();
            ownerSem.V();
            throw Failed();
        }


        if(bargingSem.empty()){
            bargingFlag = false;
        }else{
            if(!bargingFlag)
                bargingSem.V();
        }
    }


    // decide the vote result
    Tour res;
    res.groupno = groupnumber;
    currentBallot.giftshop += ballot.giftshop;
    currentBallot.picture += ballot.picture;
    currentBallot.statue += ballot.statue;

    voted++;                               // update the current voted number


    if(printmode)
    printer.print(id, Voter::States::Vote, ballot);

    if(voted == group){                    // when group is formed
        bargingFlag = true;
        if(printmode)
        printer.print(id, Voter::States::Complete);

        groupnumber++;


    }else{                                 // when group is not yet formed
        if(printmode)
        printer.print(id, Voter::States::Block, voted);   // print block message

        if(bargingSem.empty()){
            bargingFlag = false;
        }else{
            bargingSem.V();
        }
        waitForGroupSem.P(ownerSem);                          // wait for the group not full sem


        if(printmode)
        printer.print(id, Voter::States::Unblock, voted - 1); // print the unblock message

        if(voterLeft < group){                 // may have issue with no catch flow
            resetlock();
            ownerSem.V();
            throw Failed();
        }

    }


    // calculate the result tour
    unsigned int maxVote = max(max(currentBallot.giftshop, currentBallot.picture), currentBallot.statue);


    if(maxVote == currentBallot.giftshop){
        res.tourkind = TourKind::GiftShop;

    }else{
        if(maxVote == currentBallot.picture){
            res.tourkind = TourKind::Picture;
        }else{
            res.tourkind = TourKind::Statue;
        }
    }

    if(!waitForGroupSem.empty()){
        waitForGroupSem.V();
    }

    voted --;

    if(voted == 0){
        resetcount();
        bargingFlag = false;
        if(!bargingSem.empty()){
            bargingSem.V();
        }


    }

    ownerSem.V();
    return res;
}

void TallyVotes::done(){

    ownerSem.P();
    // remove voter;

    voterLeft--;
    if(voterLeft < group){                 // may have issue with no catch flow
        resetlock();
    }
    ownerSem.V();
}