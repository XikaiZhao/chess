#ifndef KING_H
#define KING_H

#include "Defs.h"
#include "Board.h"

template<bool isWhite>
class King {
    int _row, _col, _ind;
public:
    King(int ind) : _ind(ind) {
        _row = ind / ncol;
        _col = ind % ncol; 
    }

    // return number of pieces that are checking the king
    // if checkingPiece is not nullptr, it will be updated with the index of the last checking piece found
    int isChecked(const std::string& board, int* checkingPiece = nullptr);

    // update pieces that are checking the king
    void getPinnedPieces(const std::string& board, std::vector<int64_t>& pinnedPieces);

    void getLegalMoves(const Board &board, std::vector<Move> &legalMoves, int pieceIndexKingChecked);

private:
    int isCheckedImpl(const std::string& board, int row, int col, int* checkingPiece = nullptr);
    int isCheckedHelper1(const std::string& board, char type, int row, int col, int rowDelta, int colDelta, int* checkingPiece = nullptr);
    int isCheckedHelper2(const std::string& board, bool checkdiag, int row, int col, int rowDelta, int colDelta, int* checkingPiece = nullptr);

    void pinCheckHelper(const std::string &board, bool checkDiag, int rowDelta, int colDelta, int64_t &pinned);
};

#endif // KING_HG