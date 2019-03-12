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

    bool printprint = false;
    ownerSem.P();                  //acquire owner sem
    //cout<<"id : " << id << " acquire owner "<<endl;

    if(voterLeft < group){                 // when need to
        resetlock();
        ownerSem.V();
        throw Failed();
    }

    if(bargingFlag){                       // if barging flag is up, acquire the barging lock
       // cout<<"id : " << id << " barging "<<endl;
        if(printmode)
        printer.print(id, Voter::States::Barging); // print the barging message
        ownerSem.V();
        bargingSem.P();
        ownerSem.P();
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

        if(printmode)
        printer.print(id, Voter::States::Complete);

        groupnumber++;


    }else{                                 // when group is not yet formed
        if(printmode)
        printer.print(id, Voter::States::Block, voted);   // print block message

        if(bargingSem.empty()){
            if(printprint)
                cout<<"id : "<<id<<" barging down "<<endl;
            bargingFlag = false;
        }else{
            //cout<<"id : " << id << " release barging"<<endl;
            bargingSem.V();
        }
        //cout<<"id : " << id << " waiting for vote "<<endl;
        ownerSem.V();
        waitForGroupSem.P();                          // wait for the group not full sem
        ownerSem.P();



        //cout<<"id : " << id << " got vote "<<endl;
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

    if(printprint)
     cout<<"id process : " << id << " "<<waitForGroupSem.empty()<<endl;
    if (waitForGroupSem.empty()){

            resetcount();

        if (bargingSem.empty()) {
            if(printprint)
                cout<<"id : "<<id<<" barging down part 2 "<<endl;
            bargingFlag = false;
        } else {
            bargingSem.V();
        }


        if (voterLeft < group) {                 // may have issue with no catch flow
            if(printprint)
                cout<<"1 id : " << id << " raise on voterleft : "<<voterLeft<< " voted left: "<<voted<<endl;
            resetlock();
            ownerSem.V();
            throw Failed();
        }
    }else{
        if(printprint)
            cout<<"id : "<<id<<" barging up part 2 "<<endl;
        bargingFlag = true;
        waitForGroupSem.V();
        if(voterLeft < group){                 // may have issue with no catch flow
            if(printprint)
                cout<<" 2 id : " << id << " raise on voterleft : "<<voterLeft<< " voted left: "<<voted<<endl;
            resetlock();
            ownerSem.V();
            throw Failed();
        }
    }

    ownerSem.V();
    return res;
}

void TallyVotes::done(){

    bool printprint = false;
    ownerSem.P();
    // remove voter;

    if(!waitForGroupSem.empty()||!bargingSem.empty()||bargingFlag){
        if(!bargingFlag){
            if(printprint)
                cout<<"done call signal on "<< voted<<endl;
            bargingSem.V();
        }else{
            if(printprint){
                cout<<"voter left : " << voted << " call barging "<< waitForGroupSem.empty()
                    <<"  " << bargingSem.empty()<<endl;}
            ownerSem.V();
            bargingSem.P();
            ownerSem.P();


            if(printprint)
                cout<<"voter left : " << voted << " release from barging"<<endl;
            if(bargingSem.empty()) {
                if(printprint)
                    cout<<"done "<<" barging down "<<endl;
                bargingFlag = false;
            }else{
                bargingSem.V();
            }
        }

    }

    voterLeft--;
    if(voterLeft < group&& voterLeft > 0){                 // may have issue with no catch flow
        resetlock();
    }
    ownerSem.V();
}