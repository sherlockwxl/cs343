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


    if(voterLeft < group){                 // when
        resetlock();
        ownerLock.release();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        if(printmode)
        printer.print(id, Voter::States::Barging); // print the barging message
        bargingLock.wait(ownerLock);


        if(bargingLock.empty()){
            //bargingFlag = false;
        }else{
            //bargingLock.signal();
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
        //bargingFlag = true;
        if(printmode)
        printer.print(id, Voter::States::Complete);

        groupnumber++;

        //waitForGroup.signal();

    }else{                                 // when group is not yet formed
        if(printmode)
        printer.print(id, Voter::States::Block, voted);   // print block message

        if(bargingLock.empty()){
            bargingFlag = false;
        }else{
            bargingLock.signal();
        }
        waitForGroup.wait(ownerLock);                          // wait for the group not full lock
       // waitForGroup.signal();
        if(printmode)
            printer.print(id, Voter::States::Unblock, voted-1); // print the unblock message

    }

    voted--;

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




    if(!waitForGroup.empty()){
        bargingFlag = true;
        waitForGroup.signal();
        if(voterLeft < group){                 // may have issue with no catch flow
            resetlock();
            ownerLock.release() ;
            throw Failed();
        }

    }else{
        //if(voted == 0){
            if(voted == 0)
            resetcount();

            if(bargingLock.empty()) {
                bargingFlag = false;
            }else{
                bargingLock.signal();
            }


        if(voterLeft < group){                 // may have issue with no catch flow
            resetlock();
            ownerLock.release() ;
            throw Failed();
        }

        //}
    }




    ownerLock.release();
    return res;



}

void TallyVotes::done(){


    ownerLock.acquire();
    // remove voter;

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        bargingLock.wait(ownerLock);


        if(bargingLock.empty()){
            bargingFlag = false;
        }else{
            bargingLock.signal();
        }
    }



    voterLeft--;
    if(voterLeft < group && voterLeft > 0){                 // may have issue with no catch flow
        resetlock();
    }
    ownerLock.release();

}