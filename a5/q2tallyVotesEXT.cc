#include "q2tallyVotes.h"
#include "q2voter.h"
#include "q2printer.h"
#include <iostream>
#include <algorithm>
using namespace std;
extern bool printmode;

// helper function to reset the voter count
void TallyVotes::resetcount(){
    currentBallot.statue = 0;
    currentBallot.picture = 0;
    currentBallot.giftshop = 0;
}

// tally voter implementation using  barrier solution
TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
        voters(voters), group(group), printer(printer){
    groupnumber = 1;
    voted = 0;
    voterLeft = voters;
    resetcount();
};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){
    if(voterLeft < group){ // when voter left cant form a group throw error
        throw Failed();
    }

    Tour res;
    //build vote result first

    res.groupno = groupnumber;
    currentBallot.giftshop += ballot.giftshop;
    currentBallot.picture += ballot.picture;
    currentBallot.statue += ballot.statue;

    voted++;

    if(printmode)
        printer.print(id, Voter::States::Vote, ballot);

    if(voted == group){// when voter number reach voter number required

        if(printmode)
            printer.print(id, Voter::States::Complete);

        groupnumber++;

    }else{             // need to block until reach voter number
        if(printmode)
            printer.print(id, Voter::States::Block, voted);

        for(;;){                // loop for accept done or vote
            _Accept(vote){      // when accept vote break loop
                if(printmode)
                    printer.print(id, Voter::States::Unblock, voted-1); // print the unblock message
                break;
            } or _Accept(done){ // when accept done keep loop and check for fail case
                if(printmode)
                    printer.print(id, Voter::States::Done);
                if(voterLeft < group){ // when voter left cant form a group throw error
                    if(printmode)
                        printer.print(id, Voter::States::Unblock, voted-1); // print the unblock message
                    throw Failed();
                }
            }
        }

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


    if(voted == 0){ // when last thread is unblocked reset counter
        resetcount();
    }

    if(voterLeft < group){ // when voter left cant form a group throw error
        throw Failed();
    }
    return res;

}


void TallyVotes::done() {  // done function only need to update the voterleft
    voterLeft--;
}