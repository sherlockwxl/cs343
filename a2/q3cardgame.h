#include <deque>
using namespace std;
class PlayInfo{// player info is used to save each play info
public:
    unsigned int id;
    unsigned int cardTook;
    unsigned int cardLeft;
    int passDirection;//1 for right, -1 for left
    bool dead;//false for not dead true for dead
    bool drunk;//false for not drunk true for drunk
    bool win;//false for not win true for win

    PlayInfo(unsigned int id, unsigned int cardTook, unsigned int cardLeft, int passDirection, bool dead,
             bool drunk, bool win) :
            id(id), cardTook(cardTook), cardLeft(cardLeft), passDirection(passDirection), dead(dead), drunk(drunk), win(win) {};
};

class Printer {
    // YOU MAY ADD PRIVATE MEMBERS
    deque<PlayInfo* > infoqueue;
    unsigned int NoOfPlayers;
    unsigned int NoOfCards;
    void flush();
    void printheader();
public:
    Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards );
    void prt( unsigned int id, int took, int RemainingPlayers );
};

_Coroutine Player {
    // YOU MAY ADD PRIVATE MEMBERS, INCLUDING STATICS
    static unsigned int totalPlayersLeft;
  private:
    unsigned int id;
    Printer &printer;
    Player *leftplayer;
    Player *rightplayer;
    unsigned int deckReceived;
    bool drunk;
    void main();
    int status;//0 for normal 1 for dead 2 for win 3 for drunk
  public:
    enum { DEATH_DECK_DIVISOR = 7 };
    static void players( unsigned int num );
    Player( Printer &printer, unsigned int id );
    void start( Player &lp, Player &rp );
    void play( unsigned int deck );
    void drink();
};

