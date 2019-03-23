#ifndef Q2_TALLYVOTER
#define Q2_TALLYVOTER
#include "uC++.h"
_Monitor Printer;


using namespace std;

#if defined( EXT )                           // external scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( INT )                         // internal scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    uCondition groupFormed;
    // private declarations for this kind of vote-tallier
#elif defined( INTB )                        // internal scheduling monitor solution with barging
// includes for this kind of vote-tallier
_Monitor TallyVotes {

    unsigned int counter;                    // counter used to count the total voted
    unsigned int currentTarget;              // current target total voter
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (variable name may be changed)
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks
#elif defined( AUTO )                        // automatic-signal monitor solution
// includes for this kind of vote-tallier
#include "AutomaticSignal.h"
_Monitor TallyVotes {

    AUTOMATIC_SIGNAL;
    void printerHelper(bool mode, unsigned int id, int type, unsigned int numBlocked);
    // private declarations for this kind of vote-tallier
#elif defined( TASK )                        // internal/external scheduling task solution
_Task TallyVotes {

    //private attributes that need to be passed between main and vote
    unsigned int newid;

    uCondition cond;

    void main();


    // private declarations for this kind of vote-tallier
#else
#error unsupported voter type
#endif

// common declarations
    unsigned int voters;             // total voter number
    unsigned int group;              // voter number required to form a group
    Printer & printer;               // reference to a printer

    unsigned int groupnumber;        // current gorup id
    unsigned int voted;              // counter for how many voters voted
    unsigned int voterLeft;          // counter for how many voters left

    void resetcount();               // helper function to reser the vote count

public:                                    // common interface
    _Event Failed {};
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum TourKind { Picture = 'p', Statue = 's', GiftShop = 'g' };
    struct Tour { TourKind tourkind; unsigned int groupno; };
    Tour vote( unsigned int id, Ballot ballot );
    void done();

private:
    Ballot currentBallot;
#if defined( TASK )
    Ballot newBallot;
    Tour res;
#endif
};

#endif