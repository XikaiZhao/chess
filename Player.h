#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <utility>
#include <vector>
#include "Board.h"
#include "Piece.h"
#include "Defs.h"

template <bool isWhite>
class Player 
{
public:
    Player(Board* board) : _board(board), _pieces(board->getPieces<isWhite>()), _king(board->getKing<isWhite>()) {}


    Player(const Player<isWhite>& other) {
        _board = other._board;
        _pieces = _board->getPieces<isWhite>();
        _king = _board->getKing<isWhite>();

    }

    void getLegalMoves(std::vector<Move >& legalMoves);
    void makeMove(const Move& move);
    //void undoLastMove() {};


private: 
    Board* _board;
    const std::vector<std::shared_ptr<Piece> >& _pieces;
    King<isWhite>* _king;
};

#endif //PLAYER_H