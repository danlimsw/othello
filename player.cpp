// CS2 Assignment 9 Part 1, Winter 2017
// Author: Soon Wei Daniel Lim

#include "player.hpp"
#include "tree.hpp"

using namespace std;
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */


Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
  //cerr << "creating Player" << endl;
  board = new Board();
  mySide = side;
  oppSide = flip(side);
  moveNumber = 0;
  //cerr << "finished creating player" << endl;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
  //cerr << "beginning to delete player" << endl;
  delete board;
  //cerr << "finished deleting player" << endl;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
  moveNumber += 1;
  if (opponentsMove == nullptr || board->isDone()) 
  {
    // then the board does not need to be updated. the opponent did not make a
    // move
    //cerr << "Received nullptr" << endl;
  }
  else 
  {
   //  cerr << "Received opp move " << opponentsMove->x << " "
     //  << opponentsMove->y << endl;
    board->doMove(opponentsMove, oppSide); 
  }
  // check if there are any legal moves
  if (!board->hasMoves(mySide)) 
  {
  //  cerr << "no moves on my side. sending nullptr." << endl;
    return nullptr;
  }
  // otherwise, we will proceed to construct the decision tree
  Tree tree = Tree();
  Node * root = new Node();
  root->board = board->copy();
  root->lastMove = nullptr;
  root->depth = 0;
  root->nextToPlay = mySide;
  tree.root = root;
  tree.currentDepth = 0;
  //cerr << "decision tree constructed" << endl;
  // now proceed to grow the tree to n generations
  //if (testingMinimax)
  //{
  //  tree.growTree(2);
  //}
  //else
  //{
  if (msLeft < 30000)
  {
    tree.growTree(4); // speed is of the essence for the last 30 seconds!
  }
  else
  {
    if (moveNumber < 50) {tree.growTree(6);}
    else {tree.growTree(8);}
    //cerr << "decision tree grown" << endl;
  }
  //}
  // get the best move from the grown tree
  Move * lastMoveSent = new Move(-1, -1);
  Move * bestMove = tree.getBestMove(mySide);
  lastMoveSent->x = bestMove->x;
  lastMoveSent->y = bestMove->y;
  // do the move on the internal board
  // cerr << "sending move " << lastMoveSent->x << " " << lastMoveSent->y << endl;
  board->doMove(lastMoveSent, mySide);
  return lastMoveSent;
}

