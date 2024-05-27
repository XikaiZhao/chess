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
class Player 
{
public:
    void getLegalMoves(const Board& board, std::vector<Move >& legalMoves);
    void makeMove(Board& board, const Move& move);
    virtual void makeMove(Board& board) {};
    // void undoLastMove() {};

protected: 
};

/////////////////////////////////////////////////////
template <bool isWhite>
class HumanPlayer : public Player<isWhite> {
    using Player<isWhite>::getLegalMoves;

public:
    void getLegalMovesWrapper(const Board& board, std::vector<Move>& legalMoves) {
        this->getLegalMoves(board, legalMoves);  // Calls the protected method
    }

    void makeMove(Board& Board) override;
};

/////////////////////////////////////////////////////
template <bool isWhite>
class RandomPlayer : public Player<isWhite> {
    using Player<isWhite>::getLegalMoves;

public:
    void getLegalMovesWrapper(const Board& board, std::vector<Move>& legalMoves) {
        this->getLegalMoves(board, legalMoves);  // Calls the protected method
    }

    void makeMove(Board& Board) override;
};

#endif //PLAYER_H