#include "q3cardgame.h"
#include "PRNG.h"
#include <iostream>
#include <unistd.h>                                     // access: getpid




int DEATH_DECK_DIVISOR = 7;
PRNG *prng;
_Event Schmilblick{};



//methods of printer
Printer::Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards):
    NoOfPlayers(NoOfPlayers), NoOfCards(NoOfCards){
    printheader();//print header each time printer is initialized
};

void Printer::flush() {

    while(infoqueue.size() != 0){//keep pop the first element in infoqueue and print the element

        PlayInfo *tempp = infoqueue.front();
        PlayInfo temp = *tempp;
        infoqueue.pop_front();

        if(temp.cardTook == 0 && !temp.win){//check if empty info
            if(!temp.drunk){//empty info
                if(infoqueue.size() !=0){//only print tab if element is not last one in queue
                    cout<<"\t";
                }

                delete tempp;
                continue;
            }else{//drunk

                cout<<"D";//print D when drunk

                if(infoqueue.size() !=0){//only print tab if element is not last one in queue
                    cout<<"\t";
                }
                delete tempp;
                continue;
            }

        }
        if(temp.win){//if player won, only deal with win when only one player left to avoid duplicate code
            if(temp.cardLeft != 0){//player win because no other players
                cout<< "#"<<temp.cardLeft<<"#";

                if(temp.dead){//handle death and win
                    cout<<"X";

                }
                if(infoqueue.size() !=0){
                    cout<<"\t";
                }
                delete tempp;
                continue;
            }



        }

        //normal output
        cout<<temp.cardTook<<":"<<temp.cardLeft;

        if(temp.win) {//either print # for win or print </> for direction.
            cout << "#";
        }else{
            cout << (temp.passDirection == 1 ? ">" : "<");
        }

        if(temp.dead){
            cout<<"X";
        }

        if(infoqueue.size() != 0){
            cout<<"\t";
        }
        delete tempp;

    }
    cout<<endl;

}

void Printer::printheader() {
    cout<<"Players: "<<NoOfPlayers<<"    "<<"Cards: "<<NoOfCards<<endl;
    for(unsigned int i = 0; i < NoOfPlayers; i++){
        cout<<"P"<<i;
        if(i != NoOfPlayers - 1){
            cout<<'\t';
        }
    }
    cout<<endl;
}

void Printer::prt(unsigned int id, int took, int RemainingPlayers) {

    for(PlayInfo* p : infoqueue){//check if the new info will overwrite any of the existing one.
        if(p != NULL){
            if(p->id == id){
                if(p->cardTook != 0){//check if the overwrite one is an empty one
                    flush();
                    break;
                }
                else if(p->cardTook == 0 && p->drunk){//check if the overwrite one is a drunk element
                    flush();
                    break;
                }
            }
        }

    }

    //add new info
    //first pad the vector with empty info
    while(infoqueue.size() < id){
        infoqueue.push_back(new PlayInfo(infoqueue.size(), 0, 0, 0, false, false, false));//MUST use heap to add new playinfo into the infoqueue
    }

    //udpate the existing playinfo
    NoOfCards-= took;//update the number of cards
    int tempdirection = NoOfCards % 2 == 0 ? 1 : -1;                       //decide the pass direction
    bool isdead = (NoOfCards + took)%DEATH_DECK_DIVISOR == 0 ? true: false;//decide if dead
    bool isdrunk = took == 0? true:false;                                  //decide if drunk
    bool iswin = (RemainingPlayers == 1 || NoOfCards == 0) ? true: false;  //decide if win

    if(id == infoqueue.size()){//if the current one should add to the end

        PlayInfo *tempinfo = new PlayInfo(id, took, NoOfCards, tempdirection, isdead, isdrunk, iswin);
        infoqueue.push_back(tempinfo);

    }else{//update existing one

        infoqueue[id]->cardTook = took;
        infoqueue[id]->cardLeft = NoOfCards;
        infoqueue[id]->passDirection = tempdirection;
        infoqueue[id]->dead = isdead;
        infoqueue[id]->drunk = isdrunk;
        infoqueue[id]->win = iswin;
    }

    if(iswin){//when win flush the buffer
        flush();
    }



}

//private method of player
void Player::players(unsigned int num){
    totalPlayersLeft = num;
}

Player::Player( Printer &printer, unsigned int id ):
    printer(printer),id(id), leftplayer(NULL), rightplayer(NULL),deckReceived(0), drunk(false){};


void Player::start( Player &lp, Player &rp ){//link the left and right player
    leftplayer = &lp;
    rightplayer = &rp;
    resume();
}

void Player::play( unsigned int deck ){//update the deck and resume
    deckReceived = deck;
    resume();
}

void Player::drink(){
    resume();
}

void Player::main(){
    suspend();//wait for player to be called
    try{
        _Enable{
            for(;;){
                if(totalPlayersLeft == 1){//only one left, win this game
                    printer.prt(id, 0, totalPlayersLeft);
                    return;
                }

                //random pick cards and can not more than deck player has

                int takeCard = min(deckReceived, (*prng)(1,8));
                printer.prt(id, takeCard, totalPlayersLeft);

                int cardRemaining = deckReceived - takeCard;

                if(cardRemaining == 0){//game end
                    return;
                }

                if(deckReceived % DEATH_DECK_DIVISOR == 0){//check if user should be removed from the game

                    totalPlayersLeft -= 1;
                    leftplayer->rightplayer = rightplayer; //remove the current player
                    rightplayer->leftplayer = leftplayer;

                }

                //alcohol test
                if(deckReceived % DEATH_DECK_DIVISOR != 0){//only check when user was not removed

                    if((*prng)(9) == 0){

                        printer.prt(id, 0, totalPlayersLeft);
                        drunk = true;
                        _Resume Schmilblick() _At *rightplayer;//raise event to let right player know
                        rightplayer->drink();
                    }
                }

                //check pass direction
                if(cardRemaining % 2 == 0){
                    rightplayer->play(cardRemaining);
                }else{
                    leftplayer->play(cardRemaining);
                }

            }
        }
    }_CatchResume(Schmilblick& ){//when catch Schmilblick event
        if(drunk){//reach start point
            drunk = false;//change the boolean back to false
        }else{//not the start point
            printer.prt(id, 0, totalPlayersLeft);//prt drunk
            _Resume Schmilblick() _At *rightplayer;//raise event to right
            rightplayer->drink();//resume
        }

    }
    catch(Schmilblick& ){
        //do nothing
    }
}
int main( int argc, char * argv[] ) {


    //set up default and ini variables
    int games = 5;
    unsigned int players;
    int cards;
    int seed = getpid();
    bool playersset = false;
    bool cardsset = false;

    //create random number generator
    prng = new PRNG(seed);

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

    prng->seed(seed);//set up seed


    for(int i = 0 ; i < games; i++ ){//loop each game

        if(!playersset){//if players number not provided, random players amount for each game
            players = (*prng)(2,10);
        }

        if(!cardsset){//if cards number not provided, random card amount for each game
            cards = (*prng)(10,200);
        }



        int startindex = (*prng)(players - 1);


        //start game;
        Printer printer(players, cards);
        Player::players(players);//ini total player number
        Player* playerarr[players];
        for(unsigned int j = 0; j < players; j++){
            playerarr[j] = new Player(printer, j);//ini each player
        }

        for(unsigned int j = 0; j < players; j++){
            playerarr[j]->start(*playerarr[(j + players - 1)%players], *playerarr[(j + 1)%players]);//link left and right players
        }


        playerarr[startindex]->play(cards);
        for(unsigned int j = 0; j < players; j++){
            delete playerarr[j];//ini each player
        }
        if(i != games - 1){
            cout<<endl<<endl;
        }

    }

    delete prng;


}
