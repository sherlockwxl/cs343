#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"
#include <iostream>
#include <algorithm>


void TallyVotes::resetcount(){
    currentBallot.statue = 0;
    currentBallot.picture = 0;
    currentBallot.giftshop = 0;
}


void TallyVotes::resetlock(){
    bargingFlag = false;
    if(!waitForGroup.empty())
    waitForGroup.broadcast();
    if(!bargingLock.empty())
    bargingLock.broadcast();
}
// tally voter implementation using  mutex/condition solution
TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    voters(voters), group(group), printer(printer){
    groupnumber = 1;
    voted = 0;
    voterLeft = voters;
    resetcount();
    numBlocked = 0;
};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){
    bool printprint = false;
    if(printprint)
    cout << " vote received : "<< id<<" "<< ballot.picture << " " << ballot.statue<<" "<< ballot.giftshop<<endl;
    ownerLock.acquire();                   //acquire owner lock

    if(printprint)

    cout << " vote id : "<< id << " got the lock"<<endl;
    if(voterLeft < group){                 // may have issue with no catch flow

        if(printprint)
        cout<< "vote fail check fail : " << id <<endl;

        resetlock();
        ownerLock.release();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
        printer.print(id, Voter::States::Barging); // print the barging message
        bargingLock.wait(ownerLock);

        if(voterLeft < group){                 // may have issue with no catch flow
            resetlock();
            ownerLock.release();
            throw Failed();
        }


        if(bargingLock.empty()){
            bargingFlag = false;
        }
    }

    if(printprint)
    cout<< "vote pass check : " << id <<endl;

    // decide the vote result
    Tour res;
    res.groupno = groupnumber;
    currentBallot.giftshop += ballot.giftshop;
    currentBallot.picture += ballot.picture;
    currentBallot.statue += ballot.statue;
    
    voted++;                               // update the current voted number



    printer.print(id, Voter::States::Vote, ballot);

    if(voted == group){                    // when group is formed
        bargingFlag = true;
        if(printprint)
        cout<<" on id : " << id << " group is formed : "<<voted << " and "<<group <<endl;
        printer.print(id, Voter::States::Complete);
        waitForGroup.signal();
        groupnumber++;


    }else{                                 // when group is not yet formed
        printer.print(id, Voter::States::Block, voted);   // print block message

        if(!bargingLock.empty()){
            bargingLock.signal();
        }else{
            bargingFlag = false;
        }
        if(printprint)
        cout << " id : "<< id << " wait for more vote" << endl;
        waitForGroup.wait(ownerLock);                          // wait for the group not full lock
        waitForGroup.signal();
        printer.print(id, Voter::States::Unblock, voted - 1); // print the unblock message

        if(voterLeft < group){                 // may have issue with no catch flow
            resetlock();
            ownerLock.release();
            throw Failed();
        }

    }



    unsigned int maxVote = max(max(currentBallot.giftshop, currentBallot.picture), currentBallot.statue);

    //cout << " max value is " << maxVote << " " << currentBallot.picture << " " << currentBallot.statue << " "
        // << currentBallot.giftshop<<endl;
    if(maxVote == currentBallot.giftshop){
        res.tourkind = TourKind::GiftShop;

    }else{
        if(maxVote == currentBallot.picture){
            res.tourkind = TourKind::Picture;
        }else{
            res.tourkind = TourKind::Statue;
        }
    }



    voted --;

    if(voted == 0){
        resetcount();
        bargingLock.signal();
        bargingFlag = false;
    }

    ownerLock.release();
    return res;



}

void TallyVotes::done(){


    ownerLock.acquire();
    // remove voter;

    voterLeft--;
    if(voterLeft < group){                 // may have issue with no catch flow
        resetlock();
    }
    ownerLock.release();

}