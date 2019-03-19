#ifndef  Q1_PRINTER
#define  Q1_PRINTER


#include <deque>
#include "q2tallyVotes.h"
#include "q2voter.h"

using namespace std;

_Monitor Printer {    // chose one of the two kinds of type constructor
        struct data{                                    // custom data struct to save printer info
            bool set;                                   // used to detect conflict and decide when to flush
            Voter::States state;
            TallyVotes::Tour tour;
            TallyVotes::Ballot ballot;
            unsigned int numBlocked;
        };

        deque<data> buffer;                             // buffer store queue(for easy pop and push operation)
        unsigned int voters;                            // number of total voters
        void flush();                                   // function called to flush the buffer
        void checkflush(unsigned int id);               // helper function to call flush if necessary
public:
        Printer( unsigned int voters );
        void print( unsigned int id, Voter::States state );
        void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
        void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot );
        void print( unsigned int id, Voter::States state, unsigned int numBlocked );
        ~Printer();
};

#endif