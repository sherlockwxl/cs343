#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"


// tally voter implementation using  mutex/condition solution
TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    voters(voters), group(group), printer(printer){ groupnumber = 1; voted = 0; voterLeft = voters;};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){

    ownerLock.acquire();                   //acquire owner lock

    if(voterLeft < group){                 // may have issue with no catch flow
        bargingFlag = false;
        waitForGroup.broadcast();
        bargingLock.broadcast();
        ownerLock.release();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        printer.print(id, Voter::States::Barging); // print the barging message
        bargingLock.wait(ownerLock);
        if(bargingLock.empty()){
            bargingFlag = false;
        }
    }

    

    // decide the vote result
    Tour res;
    currentBallot.giftshop += ballot.giftshop;
    currentBallot.picture += ballot.picture;
    currentBallot.statue += ballot.statue;
    
    voted++;                               // update the current voted number

    unsigned int maxVote = max(max(currentBallot.giftshop, currentBallot.picture), currentBallot.statue);
    res.groupno = groupnumber;
    if(maxVote == currentBallot.giftshop){
        res.tourkind = TourKind::GiftShop;

    }else{
        if(maxVote == currentBallot.picture){
            res.tourkind = TourKind::Picture;
        }else{
            res.tourkind = TourKind::Statue;
        }
    }


    printer.print(id, Voter::States::Vote,ballot);

    if(voted == group){                    // when group is formed
        printer.print(id, Voter::States::Complete);
        waitForGroup.signal();
        groupnumber++;

    }else{                                 // when group is not yet formed
        printer.print(id, Voter::States::Block, numBlocked);   // print block message
        waitForGroup.wait(ownerLock);                          // wait for the group not full lock
        printer.print(id, Voter::States::Unblock, numBlocked); // print the unblock message

    }

    voted --;



    return res;



}

void TallyVotes::done(){
    ownerLock.acquire();
    // remove voter;

    voterLeft--;
    if(voterLeft < group){                 // may have issue with no catch flow
        bargingFlag = false;
        waitForGroup.broadcast();
        bargingLock.broadcast();
        ownerLock.release();
        throw Failed();
    }
    ownerLock.release();

}