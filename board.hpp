#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
    bitset<64> getStableArray(); // checks if pieces are stable just by looking
                                 // at filled rows and columns
    bool isYFull(int x); 
    bool isXFull(int y);
public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    int getWhiteValue(); // this function gets the current value of the board
                         // from white's perspective. this considers edge
                         // and corners to be more valuable than intermediate
                         // pieces
};

#endif
