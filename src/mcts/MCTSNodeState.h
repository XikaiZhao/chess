#ifndef MCTSSTATE_H
#define MCTSSTATE_H

#include <vector>
template<typename MoveType>
class MCTSNodeState
{
public:
  virtual void makeMove(MoveType move) {};
  //virtual void undoMove(MoveType move, MoveType parent_move) = 0;
  virtual bool isTerminal() {return false;};
  //virtual float getScore() = 0;
  virtual int getCurrentPlayer() {return 0;};
  virtual float randomSimulate() {return 0.0f;};
  virtual float adjustScore(float score) {return score;};
  virtual const std::vector<MoveType>& getPossibleMoves() {return _allValidMoves;}; // MCTS will loop through all possible moves to find the best one
  virtual int getPossibleMovesCount() {return _allValidMoves.size();};
  virtual MoveType getParentMove() {return _parentMove;};
  virtual void printState() {};

  // MCTS will call = operator to copy the state, so = operator might need to be overriden

protected:
  MoveType _parentMove;
  std::vector<MoveType> _allValidMoves;
};

#endif //MCTSSTATE_H
