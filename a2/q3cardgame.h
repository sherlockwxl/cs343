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
    deque<PlayInfo *> infoqueue;/*use deque to store playinfo, easy for access by index, also dont have to
                                 *take care of info input order, since we will always pad the missing on
                                 * in front. Eg: when we add playinfo for player with id 3. We will first make
                                 * sure we have 3 playinfo(for id: 0, 1 ,2) already in the queue. So we could just
                                 * add playinfo for the player with id 3. If next playinfo is for player 2. We can just
                                 * check if the playinfo 2 is an empty one or not, if so we just update the existing
                                 * one otherwise it means we need a flush. after flush id done, we add the new playinfo
                                 * into the queue(with empty info for 0 and 1 in front)
                                 * */

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
    bool drunk;  //bool is used to stop the event loop 
    void main();
  public:
    enum { DEATH_DECK_DIVISOR = 7 };
    static void players( unsigned int num );
    Player( Printer &printer, unsigned int id );
    void start( Player &lp, Player &rp );
    void play( unsigned int deck );
    void drink();
};

unsigned int Player::totalPlayersLeft;

