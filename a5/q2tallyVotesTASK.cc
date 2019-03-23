#include "q2tallyVotes.h"
#include "q2voter.h"
#include "q2printer.h"
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
        voters(voters), group(group), printer(printer){
    groupnumber = 1;
    voted = 0;
    voterLeft = voters;
    resetcount();
};

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ){
    newid = id;
    newBallot = ballot;
    cond.wait(newid);
    if(voterLeft < group){ // when voter left cant form a group throw error
        throw Failed();
    }
    return res;
}
void TallyVotes::done() {

    voterLeft--;
}

void TallyVotes::main(){
    for(;;){
         _Accept(vote){

            voted++;
            res.groupno = groupnumber;
            currentBallot.giftshop += newBallot.giftshop;
            currentBallot.picture += newBallot.picture;
            currentBallot.statue += newBallot.statue;

            if(printmode)
                printer.print(newid, Voter::States::Vote, newBallot);
            if(printmode)
                printer.print(newid, Voter::States::Block, voted);

            if(voted == group){ // when group is formed

                if(printmode)
                    printer.print(newid, Voter::States::Complete); // print the unblock message

                groupnumber ++;
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

                // now need to unblock all voted

                for(unsigned int i = 0 ; i < group; i++){
                    unsigned int forntId = cond.front();
                    if(printmode)
                        printer.print(forntId, Voter::States::Unblock, voted-1); // print the unblock message

                        voted--;
                    cond.signalBlock();
                }

                resetcount();
            }
        }
        or _Accept(done){
            //cout<<" done called and new id is "<<newid <<" waiting voted is "<<voted<< endl;
            for(unsigned int i = 0 ; i < voted; i++){
                if(!cond.empty()){

                    unsigned int forntId = cond.front();
                    //cout<<" now "<<newid<<" will wake up "<<forntId<<endl;
                    if(printmode)
                        printer.print(forntId, Voter::States::Done);

                    if(voterLeft < group){ // when voter left cant form a group throw error
                        if(printmode)
                            printer.print(forntId, Voter::States::Unblock, voted-1); // print the unblock message
                        cond.signalBlock();
                    }
                }else{
                    if(printmode)
                        printer.print(newid, Voter::States::Done);
                }

            }


        }

        or _Accept(~TallyVotes){
            done();
            return;
         }

    }
}
