#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"
#include <iostream>
#include <algorithm>
using namespace std;
extern bool printmode;


// helper fucntion to reset the three count every time
void TallyVotes::resetcount(){
    currentBallot.statue = 0;
    currentBallot.picture = 0;
    currentBallot.giftshop = 0;
}

// helper function to release the two lock current holding
void TallyVotes::resetlock(){
    bargingFlag = false;
    if(!waitForGroup.empty())
    waitForGroup.signal();
    if(!bargingLock.empty())
    bargingLock.signal();
}

// tally voter implementation using  mutex/condition solution
TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    voters(voters), group(group), printer(printer){
    groupnumber = 1;
    voted = 0;
    voterLeft = voters;
    resetcount();
};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){

    ownerLock.acquire();                   //acquire owner lock


    if(voterLeft < group){                 // if voter left is less than total required throw failed
        resetlock();
        ownerLock.release();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        if(printmode)
        printer.print(id, Voter::States::Barging); // print the barging message
        bargingLock.wait(ownerLock);

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

        if(bargingLock.empty()){                          // should allow barging

            bargingFlag = false;
        }else{
            bargingLock.signal();
        }

        waitForGroup.wait(ownerLock);                          // wait for the group not full lock

        if(printmode)
            printer.print(id, Voter::States::Unblock, voted-1); // print the unblock message

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



    if(!waitForGroup.empty()){                 // check if still voter waiting to be unblock

        bargingFlag = true;                    // disallow barging if still voter waiting
        waitForGroup.signal();

        if(voterLeft < group){                 // check for fail case

            resetlock();
            ownerLock.release() ;
            throw Failed();
        }

    }else{                                     // when not voter waiting to bu unblock should allow barging

        resetcount();                          // reset the voter count

        if(bargingLock.empty()) {              // allow barging

            bargingFlag = false;
        }else{

            bargingLock.signal();
        }

        if(voterLeft < group){                 // check for fail case

            resetlock();
            ownerLock.release() ;
            throw Failed();
        }

    }

    ownerLock.release();
    return res;                                // return vote result



}

void TallyVotes::done(){

    ownerLock.acquire();

    // remove voter;
    if(!waitForGroup.empty()||!bargingLock.empty()||bargingFlag){    // when still voter blocking or barging
        if(!bargingFlag){                                            // if barging is allowed

            bargingLock.signal();                                    // unblock barger

        }else{                                                       // when barging is not allowed pretend barge

            bargingLock.wait(ownerLock);                             // wait on barging lock

            if(bargingLock.empty()) {                                // unblock other barger
                bargingFlag = false;
            }else{
                bargingLock.signal();
            }
        }

    }

    voterLeft--;                                                     // remove current voter

    if(voterLeft < group && voterLeft > 0 ){                         // check for fail case

        resetlock();
    }

    ownerLock.release();

}