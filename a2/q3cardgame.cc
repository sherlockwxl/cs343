#include "q3cardgame.h"
#include "PRNG.h"
#include <iostream>
#include <unistd.h>                                     // access: getpid


//todo : check if input is integer


int DEATH_DECK_DIVISOR = 7;
PRNG *prng;
_Event DrinkStart{};



//methods of printer
Printer::Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards):
    NoOfPlayers(NoOfPlayers), NoOfCards(NoOfCards){};

void Printer::flush() {
    while(infoqueue.size() != 0){
        PlayInfo temp = infoqueue.pop();
        if(temp.cardTook == 0 && !temp.win){//check if empty info
            if(!temp.drunk){//empty info

            }else{//drunk
                cout<<"D";
            }

        }
        if(temp.win){//if player won, only deal with win when only one player left to avoid duplicate code
            if(temp.cardLeft != 0){//player win because no other players
                cout<< "#"<<temp.cardLeft<<"#";
            }
            if(temp.dead){//handle death and win
                cout<<"X";
            }

        }
        //normal output
        cout<<temp.cardTook<<":"<<temp.cardLeft;

        if(temp.win) {//either print # for win or print </> for direction.
            cout << "#";
        }else{
            cout << (temp.passDirection == 1) ? ">" : "<";
        }

        if(temp.dead){
            cout<<"X";
        }

        if(infoqueue.size() != 0){
            cout<<"\t";
        }

    }
    cout<<endl;

}

void Printer::printheader() {
    cout<<"Players: "<<NoOfPlayers<<"\t"<<"Cards: "<<NoOfCards<<endl;
    for(unsigned int i = 0; i < NoOfPlayers; i++){
        cout<<"P"<<i<<'\t';
    }
    cout<<endl;
}

void Printer::prt(unsigned int id, int took, int RemainingPlayers) {
    for(std::deque<T>::iterator it = infoqueue.begin(); it != infoqueue.end(); ++it) {
            if(it.id == id) {//when target id has been update
                flush();
            }

    }
    //add new info
    //first pad the vector with empty info
    while(infoqueue.size() < id){
        infoqueue.push(new Info(info.size(), 0, 0, 0, false, false, false));
    }

    int tempdirection = NoOfCards % 2 == 0 ? 1 : -1;
    bool isdead = (NoOfCards + took)DEATH_DECK_DIVISOR == 0 ? true: false;
    bool isdrunk = took == 0? true:false;
    bool iswin = (RemainingPlayers == 1 || NoOfCards == 0) ? true: false;

    if(id == info.size()){//if the current one should add to the end

        PlayInfo tempinfo = new PlayInfo(i, took, NoOfCards, tempdirection, isdead, isdrunk, iswin);

    }else{//update existing one

        infoqueue[id].cardTook = took;
        infoqueue[id].cardLeft = NoOfCards;
        infoqueue[id].passDirection = tempdirection;
        infoqueue[id].dead = isdead;
        infoqueue[id].drunk = isdrunk;
        infoqueue[id].win = iswin;
    }

    if(iswin){
        flush();
    }



}

//private method of player
static void Player::players(unsigned int num){
    total_players = num;
}

Player::Player( Printer &printer, unsigned int id ){
    printer = printer;
    id = id;
    leftplayer = NULL;
    rightplayer = NULL;
    deckReceived = 0;
    drunk = false;
}

void Player::start( Player &lp, Player &rp ){
    leftplayer = &lp;
    rightplayer = &rp;
    resume();
}

void Player::play( unsigned int deck ){
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
                //random pick cards
                int takeCard = min(deckReceived, prng(1,8));
                print.prt(id, takeCard, totalPlayersLeft);

                int cardRemaining = deckReceived - takeCard;
                if(cardRemaining == 0){//game end
                    return;
                }
                if(cardRemaining % DEATH_DECK_DIVISOR == 0){
                    totalPlayersLeft -= 1;
                    leftplayer.rightplayer = rightplayer;
                    rightplayer.leftplayer = leftplayer;
                }
                //alcohol test
                if((*prng)(0,9) == 0){
                    drunk = true;
                    _Resume DrinkStart() At *rightplayer;
                }
                //check pass direction
                if(cardRemaining % 2 == 0){
                    right.play(cardRemaining);
                }else{
                    left.play(cardRemaining);
                }

            }
        }
    }_CatchResume(DrinkStart& d){
        if(drunk){//reach start point
            drunk = false;
        }else{
            print.prt(id, 0, totalPlayersLeft);
        }
        _Resume DrinkStart() At *rightplayer;
        right.drink();
    }
    catch(DrinkStart& d){
        //
    }
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

        int startindex = prng(0, players - 1);


        //start game;
        Printer printer(players, cards);
        Player::players(players);//ini total player number
        Player* playerarr[players];
        for(unsigned int j = 0; j < players; j++){
            playerarr[j] = new Player(printer, j);//ini each player
        }

        for(unsigned int j = 0; j < players; j++){
            playerarr[j].start(playerarr[(j + palyers - 1)%players], playerarr[(j + 1)%players]);//link left and right players
        }


        playerarr[startindex].play(cards);
        for(unsigned int j = 0; j < players; j++){
            delete playerarr[j];//ini each player
        }
    }


}
