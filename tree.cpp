// Decision tree class for Othello assignment Part 1
// Author: Soon Wei Daniel Lim

#include "tree.hpp"
#include <iostream>
using namespace std;

// this function returns the list of allowed moves for the board
vector<Move> getAllowedMoves(Board * board, Side side)
{
  vector<Move> moves;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      Move move(i,j);
      if (board->checkMove(&move, side))
      {
        // then it is a valid move.
        moves.push_back(move);
      }
    }
  }
  return moves;
}

Side flip(Side side) 
{
  if (side == Side::WHITE) {return Side::BLACK;}
  else {return Side::WHITE;}
}

Node::Node()
{
// constructor for Node
board = nullptr;
lastMove = new Move(-1, -1);
depth = 0;
}

void Node::growNode(int numGen) 
{
  assert(numGen > -1);
  if (numGen == 0 || board->isDone()) 
  {
    // then there is no need to grow further children
    return;
  }
  Side side = nextToPlay;
  assert(children.empty()); // ensure that this is a node without children
  vector<Move> moves = getAllowedMoves(board, side);
  Node * temp;
  
  for (int i = 0; i < (int) moves.size(); i++)
  {
    temp = new Node();
    temp->board = board->copy();
    // perform the possible move
    (temp->board)->doMove(&(moves[i]), side);
    //cerr << "after move is performed, W B is " << temp->board->countWhite()
    //     << " " << temp->board->countBlack() << endl;
    temp->lastMove->x = moves[i].x;
    temp->lastMove->y = moves[i].y;
    temp->depth = depth + 1;
    temp->parent = this;
    temp->nextToPlay = flip(side);
    //cerr << "new board created, depth = " << temp->depth << endl;
    children.push_back(temp);
  } 
  // if more generations need to be created, call the function recursively 
  // on the children
  if (numGen > 1) {
    for (int i = 0; i < (int) children.size(); i++) 
    {
      (children[i])->growNode(numGen - 1);
    }
  }
}

int Node::getNodeScore(Side mySide) 
{
  if (children.empty()) 
  {
    // then just return the score of the current board
    // int numWhite = board->countWhite();
    // int numBlack = board->countBlack();
    int score;
    if (mySide == Side::WHITE) 
    {
      score = board->getWhiteValue();
    }
    else 
    {
      score = -board->getWhiteValue();
    }
    return score;
  }
  else if (mySide != nextToPlay)
  { // then the opponent should pick the option that is most detrimental to me
    // if there are children, check whose turn it is
   //  return the minimum score of all the children
    vector<Node*>::iterator it;
    int min = 1000000; // dummy integer
    int childScore;
    for (it = children.begin(); it != children.end(); it++) 
    {
      childScore = (*it)->getNodeScore(mySide);
      if (childScore < min) 
      {
        min = childScore;  
      }
    } 
    assert(childScore != 1000000); // ensure that the childScore was updated
    return childScore;
  }
 else
  { // then i should pick the option that is most beneficial to me
    vector<Node*>::iterator it;
    int max = -1000000; // dummy integer
    int childScore;
    for (it = children.begin(); it != children.end(); it++) 
    {
      childScore = (*it)->getNodeScore(mySide);
      if (childScore > max) 
      {
        max = childScore;  
      }
    } 
    assert(childScore != -1000000); // ensure that the childScore was updated
    return childScore;
  }
}

Node::~Node() 
{
  //cerr << "beginning of node release, depth = " << depth << endl;
  //cerr << "W and B count: " << board->countWhite() << " " 
    //   << board->countBlack() << endl;
  if (!children.empty())
  {
  // delete items associated with children
    vector<Node*>::iterator it;
    for (it = children.begin(); it != children.end(); it++) 
    {
      (*it)->~Node();
    }
  //cerr << "emptied children" << endl;
  }

  /* once the children vector has been emptied, update the parent
  if (parent != nullptr) {
    vector<Node*> newChildren; 
    vector<Node*>::iterator itp; 
    for (itp = parent->children.begin(); 
         itp != parent->children.end(); itp++) 
    {
      if (*itp != this)
      {
        newChildren.push_back(*itp);
      }
    }
    parent->children = newChildren;
    cerr << "updated parent" << endl;
  }
  */
  // finally delete the items associated with "node" itself
  if (board != nullptr) 
  {
    //cerr << "trying to delete board" << endl;
    // check if the board exists
    //cerr << "this board has black count " << board->countBlack() << endl;
    //cerr << "this board has white count " << board->countWhite() << endl;
    //cerr << "this board is done? " << board->isDone() << endl;
    delete board;
    //cerr << "board deleted" << endl;
  }
  if (lastMove != nullptr) {delete lastMove;}
  //cerr << "end of node release" << endl;
}

Tree::Tree() 
{
  root = nullptr;
  currentDepth = 0;
  bestMove = new Move(-1, -1);
}

Tree::~Tree() 
{
  //cerr << "beginning of tree release" << endl;
  if (root != nullptr) {
    root->~Node();
  }
  delete bestMove;
  //cerr << "end of tree release" << endl;
}

void Tree::growTree(int numGen) 
{
  // this always starts at the root!
  assert(root != nullptr);
  root->growNode(numGen);
}

Move * Tree::getBestMove(Side side) 
{
  // before calling this function, the tree must have already been grown!
  if (!(root->board)->hasMoves(side))
  {
    // if the root board has no moves, there isn't a best move
    return nullptr;
  }
  else 
  {
    assert(!root->children.empty()); // if there are valid moves, there must be
                                     // children in the root node
    vector<Node*>::iterator it;
    int max = -1000000; // dummy value
    int testmax;
    for (it = root->children.begin(); it != root->children.end(); it++) 
    {
      testmax = (*it)->getNodeScore(side);
      if (testmax > max) 
      {
        bestMove->x = ((*it)->lastMove)->x;
        bestMove->y = ((*it)->lastMove)->y;
        max = testmax;
      }
    }
    assert(max != -1000000); // ensure that min has been updated
    // return the move associated with the minimum gain
    return bestMove; 
  }
}
