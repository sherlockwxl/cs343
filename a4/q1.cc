int main( int argc, char * argv[] ) {


    //set up default and ini variables
    int processors = 1;
    int seed = getpid();
    int votes = 1;
    int group = 3;
    int voters = 6;

    //create random number generator
    prng = new PRNG(seed);

    try {                                               // process command-line arguments
        switch ( argc ) {
            case 6: if ( strcmp( argv[5], "d" ) != 0 ) {  // default ?
                    processors = stoi( argv[5] ); if ( processors <= 0 ) throw 1;
                } //if
            case 5: if ( strcmp( argv[4], "d" ) != 0 ) {  // default ?
                    seed = stoi( argv[4] ); if ( seed <= 0 ) throw 1;
                } //if
            case 4: if ( strcmp( argv[3], "d" ) != 0 ) {  // default ?
                    votes = stoi( argv[3] ); if ( votes <= 0) throw 1;
                } // if
            case 3: if ( strcmp( argv[2], "d" ) != 0 ) {  // default ?
                    group = stoi( argv[2] ); if ( group <= 0 ) throw 1;
                } // if
            case 2: if ( strcmp( argv[1], "d" ) != 0 ) {  // default ?
                    voters = stoi( argv[1] ); if ( voters < 0 ) throw 1;
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

    uProcessor p[processors - 1]; // number of kernel threads
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
            delete playerarr[j];//delete each player
        }
        if(i != games - 1){
            cout<<endl<<endl;
        }

    }

    delete prng;


}



