#ifndef Q1_TALLYVOTER
#define Q1_TALLYVOTER

_Monitor Printer;

using namespace std;

#if defined( MC )                    // mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {


    bool bargingFlag = false;        // barging flag
    uOwnerLock ownerLock;            // mutex lock
    uCondLock waitForGroup;          // condlock wait when number of voters are less than group required
    uCondLock bargingLock;           // barging lock
    void resetcount();
    void resetlock();
    // private declarations for this  kind of vote-tallier
#elif defined( SEM )                  // semaphore solution
#include <uSemaphore.h>
// includes for this kind of vote-tallier
class TallyVotes {

    bool bargingFlag = false;        // barging flag
    uSemaphore ownerSem;             // mutex lock
    uSemaphore waitForGroupSem;      // condlock wait when number of voters are less than group required
    uSemaphore bargingSem;           // barging lock
    void resetcount();
    void resetlock();
    // private declarations for this kind of vote-tallier
#elif defined( BAR )                // barrier solution
// includes for this kind of vote-tallier
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {

    void resetcount();
    // private declarations for this kind of vote-tallier
#else
#error unsupported voter type
#endif
// common declarations

    unsigned int voters;
    unsigned int group;
    Printer & printer;

    unsigned int groupnumber;
    unsigned int voted;
    unsigned int voterLeft;

public:                            // common interface
    _Event Failed {};
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum TourKind { Picture = 'p', Statue = 's', GiftShop = 'g' };
    struct Tour { TourKind tourkind; unsigned int groupno; };
    Tour vote( unsigned int id, Ballot ballot );
    void done();

private:
    Ballot currentBallot;

};


#endif