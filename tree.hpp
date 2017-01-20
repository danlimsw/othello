#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
#include <cassert>
#include "common.hpp"
#include "board.hpp"

// This is a decision tree class for the Othello game AI.
// The nodes in the decision tree represented by the struct Node
// Author: Soon Wei Daniel Lim

using namespace std;

struct Node {
  Board * board;
  Move * lastMove; // move that produced the current board
  int depth; // the iteration depth. depth = 0 is the board to be evaluated
  Node * parent;
  vector<Node*> children;
  Side nextToPlay; // which side is going to play next

  Node();
  ~Node();
  void growNode(int numGen);
  int getNodeScore(Side side);
};
 // Node operations
  //void growNode(Node * node, int numGen); // evaluate all possible moves for 
                           // the node and create children nodes accordingly
                           // recursively calling itself until numGen 
                           // additional generations has been grown
  //int getNodeScore(Node * node, Side side); // calculate the value for the 
                                            //board in a particular node

// Some board operations
  vector<Move> getAllowedMoves(Board * board, Side side);
  Side flip(Side side);

class Tree {
public:
  int currentDepth; // maximum depth of any node in the tree
  Node * root;
  Tree();
  ~Tree();
  Move * bestMove;
  void growTree(int numGens);
  Move * getBestMove(Side side); // returns the best move. tree must have been
                               // already grown! 
};

#endif
