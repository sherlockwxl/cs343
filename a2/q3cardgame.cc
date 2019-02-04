#include "q3cardgame.h"
#include "PRNG.h"
#include <iostream>
#include <unistd.h>                                     // access: getpid

int debug = 0;

//todo : check if input is integer


int DEATH_DECK_DIVISOR = 7;
PRNG *prng;
_Event Schmilblick{};



//methods of printer
Printer::Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards):
    NoOfPlayers(NoOfPlayers), NoOfCards(NoOfCards){
    printheader();
};

void Printer::flush() {
    if(debug)
    cout<<"flush called"<<endl;
    while(infoqueue.size() != 0){
        PlayInfo *tempp = infoqueue.front();
        PlayInfo temp = *tempp;
        infoqueue.pop_front();
        if(temp.cardTook == 0 && !temp.win){//check if empty info
            if(!temp.drunk){//empty info
                if(infoqueue.size() !=0){
                    cout<<"\t";
                }

                delete tempp;
                continue;
            }else{//drunk
                cout<<"D";
                if(infoqueue.size() !=0){
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
    if(debug)
        cout<<"flush done"<<endl;

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
    if(debug)
    cout<<"prt call id: "<< id << " took : " << took << "  reamin: "<< RemainingPlayers<<endl;
    for(PlayInfo* p : infoqueue){
        if(p != NULL){
            if(p->id == id){
                if(p->cardTook != 0){
                    flush();
                    break;
                }
                else if(p->cardTook == 0 && p->drunk){
                    flush();
                    break;
                }
            }
        }

    }
    if(debug)
        cout<<"prt first flush done  now size "<<infoqueue.size()<<endl;
    //add new info
    //first pad the vector with empty info
    while(infoqueue.size() < id){
        infoqueue.push_back(new PlayInfo(infoqueue.size(), 0, 0, 0, false, false, false));
    }
    NoOfCards-= took;
    int tempdirection = NoOfCards % 2 == 0 ? 1 : -1;
    bool isdead = (NoOfCards + took)%DEATH_DECK_DIVISOR == 0 ? true: false;
    bool isdrunk = took == 0? true:false;
    bool iswin = (RemainingPlayers == 1 || NoOfCards == 0) ? true: false;
    if(debug)
    cout<<" prt insert "<<id<<" took "<< took << " dir  "<< tempdirection<< " dead  " << isdead << " drunk "<<
        isdrunk<< " win "<< iswin<< endl;
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

    if(iswin){
        flush();
        if(debug)
            cout<<"prt win flush done  now size "<<infoqueue.size()<<endl;
    }



}

//private method of player
void Player::players(unsigned int num){
    totalPlayersLeft = num;
}

Player::Player( Printer &printer, unsigned int id ):
    printer(printer),id(id), leftplayer(NULL), rightplayer(NULL),deckReceived(0), drunk(false){};


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
                    if(debug)
                cout<<"id "<<id <<" received " << deckReceived<<endl;
                //random pick cards
                    if(debug)
                cout<<"prng called card on " <<id<<endl;
                int takeCard = min(deckReceived, (*prng)(1,8));
                printer.prt(id, takeCard, totalPlayersLeft);

                int cardRemaining = deckReceived - takeCard;
                    if(debug)
                cout<<"id "<<id <<" pick " << takeCard<< " remain " <<cardRemaining<<endl;
                if(cardRemaining == 0){//game end
                    if(debug)
                    cout<<"remain 0"<<endl;
                    return;
                }
                if(deckReceived % DEATH_DECK_DIVISOR == 0){
                    if(debug)
                    cout<<"dead"<<endl;
                    totalPlayersLeft -= 1;
                    leftplayer->rightplayer = rightplayer;
                    rightplayer->leftplayer = leftplayer;

                }
                //alcohol test
                if(deckReceived % DEATH_DECK_DIVISOR != 0){
                    if(debug)
                    cout<<"prng called drink on " <<id<<endl;
                    if((*prng)(9) == 0){
                        if(debug)
                            cout<<"id : " <<id<<" I'm drunk"<<endl;
                        printer.prt(id, 0, totalPlayersLeft);
                        drunk = true;
                        _Resume Schmilblick() _At *rightplayer;
                        rightplayer->drink();
                    }
                }

                //check pass direction
                if(cardRemaining % 2 == 0){
                    if(debug)
                    cout<<"pass right "<<cardRemaining<<endl;
                    rightplayer->play(cardRemaining);
                }else{
                    if(debug)
                    cout<<"pass left "<<cardRemaining<<endl;
                    leftplayer->play(cardRemaining);
                }

            }
        }
    }_CatchResume(Schmilblick& d){
        if(debug)
        cout<<"id : " <<id<<" I received drunk call " <<endl;
        if(drunk){//reach start point
            drunk = false;
            //printer.prt(id, 0, totalPlayersLeft);
        }else{
            printer.prt(id, 0, totalPlayersLeft);
            _Resume Schmilblick() _At *rightplayer;
            rightplayer->drink();
        }

    }
    catch(Schmilblick& d){
        //
    }
}
int main( int argc, char * argv[] ) {


    //set up default and ini variables
    int games = 5;
    unsigned int players;
    int cards;
    int seed = getpid();
    //bool gamesset = false;
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
