#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <utility>
#include <map>
#include "Defs.h"


class Board;

class Piece;
template<bool isWhite> class Pawn;
template<bool isWhite> class Rook;
template<bool isWhite> class Knight;
template<bool isWhite> class Bishop;
template<bool isWhite> class Queen;
template<bool isWhite> class King;


class Piece {
public:
    Piece() {};
    Piece(int row, int col, int id) :_id(id), _row(row), _col(col) { 
        _ind = _col + _row*ncol; 
    }
    virtual ~Piece() {};
    
    virtual Piece* clone() = 0;

    int getPosition() { return _ind; }
        
    inline void updatePosition(int ind) { 
        _row = ind/ncol, _col = ind%ncol, _ind = ind;
    }

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) = 0;


    PieceType _type = PieceType::NONE;
    bool _isWhite;
    bool _onBoard = true;
    bool _atInitialPos = true;
    int _id = -1;

    enum PinnedDirection {
        NA, HOR, VER, DIAG1 /*a1 to h8*/, DIAG2 /*a8 to h1*/
    };
    PinnedDirection _isPinned = NA;
    

protected:
    int _row, _col, _ind;

    template <bool isWhite>
    void getLegalMovesHelperFunc(Board*board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int dx, int dy);

    template <bool isWhite>
    void getLegalMovesHelper(Board*board, std::vector<Move> &legalMoves, int rowDelta, int colDelta);
    template <bool isWhite>
    void getLegalMovesKingCheckedHelper(Board*board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int rowDelta, int colDelta);
};

/////////////////////
template<bool isWhite>
class Bishop : public Piece {
public:
    Bishop(int row, int col, int id) : Piece(row, col, id) {
        _type = PieceType::BISHOP;
        _isWhite = isWhite; 
    };
    virtual Piece* clone() override { return new Bishop<isWhite>(*this); }

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) override;
};

/////////////////////
template<bool isWhite>
class Rook : public Piece {
public:
    Rook(int row, int col, int id) : Piece(row, col, id) {
        _type = PieceType::ROOK;
        _isWhite = isWhite; 
    };
    virtual Piece* clone() override { return new Rook<isWhite>(*this); }

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) override;

};

/////////////////////
template<bool isWhite>
class Queen : public Piece {
public:
    Queen(int row, int col, int id) : Piece(row, col, id) {
        _type = PieceType::QUEEN;
        _isWhite = isWhite; 
    };
    virtual Piece* clone() override { return new Queen<isWhite>(*this); }

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) override;
};

/////////////////////
template<bool isWhite>
class Knight : public Piece {
public:
    using Piece::_row;
    using Piece::_col;
    using Piece::_ind;
    
    Knight(int row, int col, int id) : Piece(row, col, id) {
        _type = PieceType::KNIGHT;
        _isWhite = isWhite; 
    };
    virtual Piece* clone() override { return new Knight<isWhite>(*this); }

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) override;

private:
    static const int rowDelta[8];
    static const int colDelta[8];

    void getLegalMovesHelper(Board*board, std::vector<Move> &legalMoves, int /*rowDelta*/, int /*colDelta*/);
    void getLegalMovesKingCheckedHelper(Board*board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/);
};

/////////////////////
template<bool isWhite>
class Pawn : public Piece {
public:
    using Piece::_row;
    using Piece::_col;
    using Piece::_ind;

    Pawn(int row, int col, int id) : Piece(row, col, id) {
        _type = PieceType::PAWN;
        _isWhite = isWhite;
    };
    virtual Piece* clone() override { return new Pawn<isWhite>(*this); }

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) override;

private:
    void getLegalMovesHelper(Board*board, std::vector<Move> &legalMoves, int /*rowDelta*/, int /*colDelta*/);
    void getLegalMovesKingCheckedHelper(Board*board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/);

    void addPromotionMoves(std::vector<Move> &legalMoves, int index, int newPos);
};

/////////////////////
template<bool isWhite>
class King : public Piece {
public:
    using Piece::_row;
    using Piece::_col;
    using Piece::_ind;

    King(int row, int col, int id) : Piece(row, col, id) {
        _type = PieceType::KING;;
        _isWhite = isWhite;
    };
    virtual Piece* clone() override { return new King<isWhite>(*this); }

    // update pieces that are checking the king
    void updatePinnedPieces(Board*board);
    
    // return number of pieces that are checking the king
    // if checkingPiece is not nullptr, it will be updated with the index of the last checking piece found
    int isChecked(Board* board, int* checkingPiece = nullptr);

    virtual void getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1) override;

private:
    void getLegalMovesHelper(Board*board, std::vector<Move> &legalMoves, int rowDelta, int colDelta);
    void getLegalMovesKingCheckedHelper(Board*board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int rowDelta, int colDelta) {}

    int isCheckedHelper1(Board* board, PieceType type, int rowDelta, int colDelta, int* checkingPiece = nullptr);
    int isCheckedHelper2(Board* board, bool checkdiag, int rowDelta, int colDelta, int* checkingPiece = nullptr);
    void pinCheckHelper(Board* board, bool checkDiag, PinnedDirection dir, int rowDelta, int colDelta);
};

#endif // PIECE_H