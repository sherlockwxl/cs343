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


    ownerSem.P();                          //acquire owner sem

    if(voterLeft < group){                 // check fail case
        resetlock();
        ownerSem.V();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock

        if(printmode)
        printer.print(id, Voter::States::Barging); // print the barging message

        ownerSem.V();                      // barge release mutex lock first then acquire on barging sem
        bargingSem.P();
        ownerSem.P();

    }


    // ini the vote result
    Tour res;
    res.groupno = groupnumber;
    currentBallot.giftshop += ballot.giftshop;
    currentBallot.picture += ballot.picture;
    currentBallot.statue += ballot.statue;

    voted++;                               // update the current voted number


    if(printmode)
    printer.print(id, Voter::States::Vote, ballot);

    if(voted == group){                    // when group is formed

        if(printmode)
        printer.print(id, Voter::States::Complete);

        groupnumber++;


    }else{                                                // when group is not yet formed
        if(printmode)
        printer.print(id, Voter::States::Block, voted);   // print block message

        if(bargingSem.empty()){                           // should allow barging
            bargingFlag = false;
        }else{

            bargingSem.V();
        }

        ownerSem.V();
        waitForGroupSem.P();                          // wait for the group not full sem
        ownerSem.P();


        if(printmode)
        printer.print(id, Voter::States::Unblock, voted - 1); // print the unblock message

    }

    voted--;

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

    if (waitForGroupSem.empty()){                // when not voter waiting to bu unblock should allow barging

            resetcount();                        // reset the voter count

        if (bargingSem.empty()) {                // allow barging

            bargingFlag = false;
        } else {
            bargingSem.V();
        }


        if (voterLeft < group) {                 // may have issue with no catch flow

            resetlock();
            ownerSem.V();
            throw Failed();
        }
    }else{

        bargingFlag = true;                      // disallow barging if still voter waiting
        waitForGroupSem.V();
        if(voterLeft < group){                   // check for fail case

            resetlock();
            ownerSem.V();
            throw Failed();
        }
    }

    ownerSem.V();
    return res;
}

void TallyVotes::done(){

    ownerSem.P();
    // remove voter;

    // when still voter blocking or barging
    if(!waitForGroupSem.empty()||!bargingSem.empty()||bargingFlag){

        if(!bargingFlag){                                 // if barging is allowed

            bargingSem.V();                               // unblock barger

        }else{                                            // when barging is not allowed pretend barge

            ownerSem.V();                                 // wait on barging lock
            bargingSem.P();
            ownerSem.P();

            if(bargingSem.empty()) {                      // unblock other barger

                bargingFlag = false;
            }else{
                bargingSem.V();
            }
        }
    }

    voterLeft--;                                          // remove current voter
    if(voterLeft < group&& voterLeft > 0){                // check for fail case
        resetlock();
    }
    ownerSem.V();
}