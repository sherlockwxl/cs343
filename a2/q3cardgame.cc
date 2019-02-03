#include "q3cardgame.h"
#include "PRNG.h"
#include <iostream>
#include <unistd.h>                                     // access: getpid

//todo : check if input is integer
using namespace std;


int PlayInfo::Info(unsigned int id, unsigned int cardTook, unsigned int cardLeft, int passDirection, bool dead,
                   bool drunk, bool win) :
    id(id), cardTook(cardTook), cardLeft(cardLeft), passDirection(passDirection), dead(dead), drunk(drunk), win(win) {};


Printer::Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards):
    NoOfPlayers(NoOfPlayers), NoOfCards(NoOfCards){};

void Printer::prt(unsigned int id, int took, int RemainingPlayers) {
    
}

int main( int argc, char * argv[] ) {

    istream *infile = &cin;

    //set up default and ini variables
    int games = 5;
    int players;
    int cards;
    int seed = getpid();
    boolean gamesset = false;
    boolean playersset = false;
    boolean cardsset = false;

    //create random number generator
    prng = new PRNG();

    try {                                               // process command-line arguments
        switch ( argc ) {
            case 5: if ( strcmp( argv[4], "d" ) != 0 ) {  // default ?
                    seed = stoi( argv[4] ); if ( seed <= 0 ) throw 1;
                } //if
            case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
                    cards = stoi( argv[3] );
                    cardsset = true;
                    if ( cards < 0) throw 1;
                } // if
            case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
                    players = stoi( argv[2] );
                    playersset = true;
                    if ( players < 2 ) throw 1;
                } // if
            case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                    games = stoi( argv[1] ); if ( games < 0 ) throw 1;
                } // if
            case 1: break;                                // use all defaults
            default: throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0] << " [ games (>=0) | 'd'"
                                        "(default 5) [ players (>=2) | 'd' (random 2-10) [ cards (>0) | 'd'"
                                        "(random 10-200) [ seed (>0) | 'd' (random) ] ] ] ]" << endl;
        exit( EXIT_FAILURE );
    } // try

    prng.seed(seed);//set up seed


    for(int i = 0 ; i < games; i++ ){//loop each game

        if(!cardsset){//if cards number not provided, random card amount for each game
            cards = prng(10,200);
        }

        if(!playersset){//if players number not provided, random players amount for each game
            players = prng(2,10);
        }


    }


}
