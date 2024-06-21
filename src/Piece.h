#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>
#include <memory>
#include "Defs.h"
#include "Board.h"


class Piece;
template<bool isWhite> class Pawn;
template<bool isWhite> class Rook;
template<bool isWhite> class Knight;
template<bool isWhite> class Bishop;
template<bool isWhite> class Queen;


class Piece {
protected:
    enum PinnedDir {
        NA, HOR, VER, DIAG1 /*a1 to h8*/, DIAG2 /*a8 to h1*/
    };

    static PinnedDir getPinnedDir(int ind, std::vector<int64_t>& pinnedPieces){
        int64_t mask = 1LL << ind;
        if (pinnedPieces[0] & mask) 
            return HOR;
        else if (pinnedPieces[1] & mask)
            return VER;
        else if (pinnedPieces[2] & mask)
            return DIAG1;
        else if (pinnedPieces[3] & mask)
            return DIAG2; 
        return NA;
    }

    template <bool isWhite>
    static void getLegalMovesHelperFunc(int ind, PinnedDir pinnedDir, const Board& board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int dx, int dy);
    template <bool isWhite>
    static void getLegalMovesHelper(int ind, PinnedDir pinnedDir, const std::string& board, std::vector<Move> &legalMoves, int rowDelta, int colDelta);
    template <bool isWhite>
    static void getLegalMovesKingCheckedHelper(int ind, PinnedDir pinnedDir, const std::string& board, std::vector<Move> &legalMoves, int kingPos, int pieceIndexKingChecked, int rowDelta, int colDelta);
};


/////////////////////
template<bool isWhite>
class Bishop : public Piece {
public:
    static void getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1);
};

/////////////////////
template<bool isWhite>
class Rook : public Piece {
public:
    static void getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1);
};

/////////////////////
template<bool isWhite>
class Queen : public Piece {
public:
    static void getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1);
};

/////////////////////
template<bool isWhite>
class Knight : public Piece {
public:
    static void getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1);

private:
    static const int rowDelta[8];
    static const int colDelta[8];
    static void getLegalMovesHelperFunc(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int dx, int dy);
    static void getLegalMovesHelper(int ind, Piece::PinnedDir pinnedDir, const std::string& board, std::vector<Move> &legalMoves, int /*rowDelta*/, int /*colDelta*/);
    static void getLegalMovesKingCheckedHelper(int ind, Piece::PinnedDir pinnedDir, const std::string& board, std::vector<Move> &legalMoves, int kingPos, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/);
};

/////////////////////
template<bool isWhite>
class Pawn : public Piece {
public:
    static void getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked = -1);

private:
    static void getLegalMovesHelperFunc(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int dx, int dy);
    static void getLegalMovesHelper(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move> &legalMoves, int /*rowDelta*/, int /*colDelta*/);
    static void getLegalMovesKingCheckedHelper(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move> &legalMoves, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/);
    static void addPromotionMoves(std::vector<Move> &legalMoves, int index, int newPos);
};

template<bool isWhite>
void getLegalMovesFactory(int ind, char type, const Board& board, std::vector<int64_t>& pinnedPieces, std::vector<Move>& getLegalMoves, int pieceIndexKingChecked = -1) {
        switch (type) {
        case 'P':
        case 'p':{ 
            Pawn<isWhite>::getLegalMoves(ind, pinnedPieces, board, getLegalMoves, pieceIndexKingChecked);
            break;
        }
        case 'N':
        case 'n': {
            Knight<isWhite>::getLegalMoves(ind, pinnedPieces, board, getLegalMoves, pieceIndexKingChecked);
            break;
        }
        case 'B':
        case 'b': {
            Bishop<isWhite>::getLegalMoves(ind, pinnedPieces, board, getLegalMoves, pieceIndexKingChecked);
            break;
        }   
        case 'R':   
        case 'r': {
            Rook<isWhite>::getLegalMoves(ind, pinnedPieces, board, getLegalMoves, pieceIndexKingChecked);
            break;
        }
        case 'Q':
        case 'q': {
            Queen<isWhite>::getLegalMoves(ind, pinnedPieces, board, getLegalMoves, pieceIndexKingChecked);
            break;
        }
        default:
            std::cerr << "Invalid piece type: " << type << std::endl;
            throw std::runtime_error("Invalid piece type");
    }
}

#endif // PIECE_H