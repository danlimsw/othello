#include "board.hpp"
#include <vector>

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
// this function gets the current value of the board from white's perspective
int Board::getWhiteValue()
{
  int cornerValue = 3; // deny corners!
  int edgeValue = 2;
  int normalValue = 1;
  int stableValue = 3;
  int blackTotal = 0;
  int whiteTotal = 0;
  // first check the non-edge non-corner locations
  for (int i = 1; i < 7; i++) 
  {
    for (int j = 1; j < 7; j++) 
    {
      if (black[i+8*j])
        // then the location has a black piece
      {
        blackTotal += normalValue;
      }
      else if (taken[i+8*j])
      {
        // then the location has a white piece
        whiteTotal += normalValue;
      }
    } 
  }
  // now check the edge locations
  for (int i = 1; i < 7; i++)
  { 
    // check top edge
    if (black[i]) {blackTotal += edgeValue;}
    else if (taken[i]) {whiteTotal += edgeValue;}
  
    // check left edge
    if (black[8*i]) {blackTotal += edgeValue;}
    else if (taken[8*i]) {whiteTotal += edgeValue;}
  
    // check bottom edge
    if (black[i+7*8]) {blackTotal += edgeValue;}
    else if (taken[i+7*8]) {whiteTotal += edgeValue;}

    // check right edge
    if (black[7+8*i]) {blackTotal += edgeValue;}
    else if (taken[7+8*i]) {whiteTotal += edgeValue;}
  }
  // now check corner locations
  if (black[0]) {blackTotal += cornerValue;}
  else if (taken[0]) {whiteTotal += cornerValue;}
  
  if (black[7]) {blackTotal += cornerValue;}
  else if (taken[7]) {whiteTotal += cornerValue;}
 
  if (black[56]) {blackTotal += cornerValue;}
  else if (taken[56]) {whiteTotal += cornerValue;}

  if (black[63]) {blackTotal += cornerValue;}
  else if (taken[63]) {whiteTotal += cornerValue;}

  // check for stability and award additional points
  bitset<64> stable = getStableArray();
  if (stable.count() != 0)
  {
    // then there are stable positions
    for (int i = 0; i < 64; i++)
    {
      if (stable[i])
      {
        if (black[i]) {blackTotal += stableValue;}
        else if (taken[i]) {whiteTotal += stableValue;}
      }
    }
  }

  return (whiteTotal - blackTotal);
}

bool Board::isYFull(int x)
{ // checks if the vertical is full at position x
  bool full = true;
  for (int y = 0; y < 8; y++)
  {
    if (!taken[x+8*y]) {full = false;}
  }
  return full;
}

bool Board::isXFull(int y)
{
  bool full = true;
  for (int x = 0; x < 8; x++)
  {
    if (!taken[x+8*y]) {full = false;}
  }
  return full;
}

bitset<64> Board::getStableArray()
{
// calculates whether each piece is currently stable
  bitset<64> stable;
  stable.reset();
  vector<int> xFull;
  vector<int> yFull;
  for (int i = 0; i < 8; i++)
  {
    if (isYFull(i)) {yFull.push_back(i);}
    if (isXFull(i)) {xFull.push_back(i);}
  }
  for (int i = 0; i < (int) xFull.size(); i++) 
  {
    for (int j = 0; j < (int) yFull.size(); j++)
    {
      stable.set(i+8*j);
    }
  }
  // add the corner pieces
  stable.set(0);
  stable.set(7);
  stable.set(56);
  stable.set(63);

  // now evaluate to check if there are any pieces surrounded by stable pieces
  // check the central region:
  for (int i = 1; i < 7; i++) 
  {
    for (int j = 1; j < 7; j++)
    {
      if (stable[(i-1)+8*(j-1)] && stable[(i-1)+8*(j+1)] 
       && stable[(i+1)+8*(j-1)] && stable[(i+1)+8*(j+1)]
       && taken[i+8*j])
      {
        // if position i,j is surrounded by stable pieces, it is stable as well
        stable.set(i+8*j);
      }
    }
  }
  // now check the edges
  for (int i = 1; i < 7; i++)
  {
    if (stable[i-1] && stable[i+1] && taken[i]) {stable.set(i);}
    if (stable[56+i-1] && stable[56+i-1] && taken[56+i]) {stable.set(56+i);}
    if (stable[8*(i-1)] && stable[8*(i+1)] && taken[8*i]) {stable.set(8*i);}
    if (stable[7+8*(i-1)] && stable[7+8*(i+1)] && taken[7+8*i])
    {
      stable.set(7+8*i);
    }
  }
  return stable;
}
