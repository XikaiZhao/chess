#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <utility>
#include <vector>
#include "Board.h"
#include "Defs.h"

template <bool isWhite>
class Player;
template <bool isWhite>
class HumanPlayer;
template <bool isWhite>
class RandomPlayer;
template <bool isWhite>
class AIPlayer;


template <bool isWhite>
class Player 
{
public:
    void makeMove(Board& board, const Move& move);
    virtual Move makeMove(Board& board, const std::vector<Move>& legalMoves) = 0;
    // void undoLastMove() {}; 
};

/////////////////////////////////////////////////////
template <bool isWhite>
class HumanPlayer : public Player<isWhite> {
public:
    Move makeMove(Board& Board, const std::vector<Move>& legalMoves) override;
};

/////////////////////////////////////////////////////
template <bool isWhite>
class RandomPlayer : public Player<isWhite> {
public:
    Move makeMove(Board& Board, const std::vector<Move>& legalMoves) override;
};

/////////////////////////////////////////////////////
template <bool isWhite>
class AIPlayer : public Player<isWhite> {
public:
    AIPlayer(float timeLimit) : _timeLimit(timeLimit) {}
    Move makeMove(Board& Board, const std::vector<Move>& legalMoves) override;

    float _timeLimit; // second
};
#endif //PLAYER_H