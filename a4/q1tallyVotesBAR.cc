#include "q1tallyVotes.h"
#include "q1voter.h"
#include "q1printer.h"
#include <iostream>
#include <algorithm>
using namespace std;
extern bool printmode;

void TallyVotes::resetcount(){
    currentBallot.statue = 0;
    currentBallot.picture = 0;
    currentBallot.giftshop = 0;
}



// tally voter implementation using  barrier solution
TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
        uBarrier(group),voters(voters), group(group), printer(printer){
        groupnumber = 1;
        voted = 0;
        voterLeft = voters;
        resetcount();

};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){

    if(voterLeft < uBarrier::total()){                         // if voter left is less than total required throw failed
        done();
        throw Failed();
    }

    if(printmode)
    printer.print(id, Voter::States::Vote, ballot);  // print vote info

    voted ++;

    Tour res;                                        // set up the return structure
    res.groupno = groupnumber;
    currentBallot.giftshop += ballot.giftshop;
    currentBallot.picture += ballot.picture;
    currentBallot.statue += ballot.statue;


    if(uBarrier::waiters() + 1 < uBarrier::total()){             // when need to wait for more voter
        if(printmode)
        printer.print(id, Voter::States::Block, voted);          // block current thread
        uBarrier::block();                                       // wait for barrier release
        printer.print(id, Voter::States::Unblock, voted - 1);    // print unblock info

    }else{                                                       // when no need to wait for more voter
        printer.print(id, Voter::States::Complete);              // print complete info
        uBarrier::block();
        groupnumber++;
    }

    if(voterLeft < uBarrier::total()){                         // if voter left is less than total required throw failed
        done();
        throw Failed();
    }


    // calculate the result tour
    unsigned int maxVote = max(max(currentBallot.giftshop, currentBallot.picture), currentBallot.statue);

    //cout << " max value is " << maxVote << " " << currentBallot.picture << " " << currentBallot.statue << " "
    // << currentBallot.giftshop<<endl;
    if(maxVote == currentBallot.giftshop){                       // handle tie cases
        res.tourkind = TourKind::GiftShop;

    }else{
        if(maxVote == currentBallot.picture){
            res.tourkind = TourKind::Picture;
        }else{
            res.tourkind = TourKind::Statue;
        }
    }


    voted --;                                                    // update voted counter

    if(voted == 0){
        resetcount();
    }

    return res;
}

void TallyVotes::done(){
    voterLeft--;
    if(voterLeft < uBarrier::total() && voterLeft > 0){                 // may have issue with no catch flow
        if(uBarrier::waiters() != 0){
            uBarrier::block();
        }
    }
}