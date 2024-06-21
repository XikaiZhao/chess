
#include <cctype>
#include "King.h"

//////////////////////////
template<bool isWhite>
int King<isWhite>::isCheckedHelper1(const std::string& board, char type, int row, int col, int rowDelta, int colDelta, int* checkingPiece) {
    int r = row + rowDelta;
    int c = col + colDelta;
    int nn = c + r * ncol;
    if (nn == _ind) return 0;

    if (r >= 0 && r < nrow && c >= 0 && c < ncol) {
        char p = board[nn];
        if (p == type) {
            if (checkingPiece != nullptr) *checkingPiece = nn;
            return 1;
        }
    }
    return 0;
}

template<bool isWhite>
int King<isWhite>::isCheckedHelper2(const std::string& board, bool checkdiag, int row, int col, int rowDelta, int colDelta, int* checkingPiece) {
    int r = row, c = col;
    while ((r += rowDelta, r >= 0 && r < nrow) && (c += colDelta, c >= 0 && c < ncol)) {
        int ind = c + r * ncol;
        char p = board[ind];
        if (p == '-' || ind == _ind)
            continue;
        else if (isupper(p) == isWhite)
            return 0;
        else{
            if ( p == (checkdiag ? (isWhite ? 'b' : 'B') : (isWhite ? 'r' : 'R')) || p == (isWhite ? 'q' : 'Q')) {
                if (checkingPiece != nullptr) *checkingPiece = ind;
                return 1;
            }
            else 
                return 0;
        }
    }
    return 0;
}

template<bool isWhite>
int  King<isWhite>::isCheckedImpl(const std::string& board, int row, int col, int* checkingPiece) {  
    int res = 0;

    // check pawn
    res += isCheckedHelper1(board, isWhite?'p':'P', row, col, isWhite?1:-1, -1, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'p':'P', row, col, isWhite?1:-1,  1, checkingPiece);
    if (!checkingPiece && res > 0)
        return res;
    
    // check knight 
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col, -1, -2, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col, -1,  2, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col,  1, -2, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col,  1,  2, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col, -2, -1, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col, -2,  1, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col,  2, -1, checkingPiece);
    res += isCheckedHelper1(board, isWhite?'n':'N', row, col,  2,  1, checkingPiece);
    if (!checkingPiece && res > 0)
        return res;

    // check bishop and queen
    res += isCheckedHelper2(board, true, row, col, -1, -1, checkingPiece); // down-left
    res += isCheckedHelper2(board, true, row, col,  1,  1, checkingPiece); // up-right
    res += isCheckedHelper2(board, true, row, col,  1, -1, checkingPiece); // up-left
    res += isCheckedHelper2(board, true, row, col, -1,  1, checkingPiece); // down-right
    if (!checkingPiece && res > 0)
        return res;

    // check rook and queen
    res += isCheckedHelper2(board, false, row, col,  0, -1, checkingPiece); // left
    res += isCheckedHelper2(board, false, row, col,  0,  1, checkingPiece); // right
    res += isCheckedHelper2(board, false, row, col, -1,  0, checkingPiece); // down
    res += isCheckedHelper2(board, false, row, col,  1,  0, checkingPiece); // up
    if (!checkingPiece && res > 0)
        return res;

    return res;
}


template<bool isWhite>
int King<isWhite>::isChecked(const std::string& board, int* checkingPiece) {
    return isCheckedImpl(board, _row, _col, checkingPiece);
}

template<bool isWhite>
void King<isWhite>::getLegalMoves(const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    int opponentKingPos = board.getKingIndex(!isWhite);
    const std::string& bd = board.getBoard();
    for (int i = -1; i <= 1; i++) {
        int newRow = _row + i;
        for (int j = -1; j <= 1; j++) {
            int newCol = _col + j;
            if (newRow >= 0 && newRow < nrow && newCol >= 0 && newCol < ncol) {
                int pos = newCol + newRow * ncol;
                char p = bd[pos];
                if (p != '-' && isupper(p) == isWhite)
                    continue;
                if ( (opponentKingPos < pos-ncol-1 || opponentKingPos > pos+ncol+1) ||
                    (opponentKingPos % ncol < newCol - 1 || opponentKingPos % ncol > newCol + 1) )      
                {
                    if (!isCheckedImpl(bd, newRow, newCol, nullptr)) 
                        legalMoves.push_back({_ind, pos});
                }
            }
        }
    }
    if (pieceIndexKingChecked >= 0)
        return;

    // castling
    auto addCastlingMove = [&](int rook_col) {
        int colDelta = (rook_col > _col) ? 1 : -1;
        int newCol = _col + colDelta*2;
        int pos = newCol + _row*ncol;
        // check if the oppoenent king is in the path of the castling move
        bool canCastle = ( (opponentKingPos < pos-ncol-1 || opponentKingPos > pos+ncol+1) ||
                      (opponentKingPos % ncol < newCol - 1 || opponentKingPos % ncol > newCol + 1) );
        // check if there are pieces between king and rook
        for (int c = _col; canCastle && (c += colDelta) > 0 && c < ncol-1; canCastle = (bd[c + _row*ncol] == '-'));
        // check if king will pass through check
        for (int c = _col; canCastle && (c += colDelta) > 1 && c < ncol-1; canCastle = (isCheckedImpl(board.getBoard(), _row, c, nullptr) == 0));
        // add castling move
        if (canCastle) 
            legalMoves.push_back(Move{_ind, pos});
    };

    if (isWhite) {
        if (board.getWhiteCanCastleKingSide())
            addCastlingMove(ncol-1);
        if (board.getWhiteCanCastleQueenSide())
            addCastlingMove(0);
    }
    else {
        if (board.getBlackCanCastleKingSide())
            addCastlingMove(ncol-1);
        if (board.getBlackCanCastleQueenSide())
            addCastlingMove(0);
    }
}

template<bool isWhite>
void King<isWhite>::pinCheckHelper(const std::string& board, bool checkDiag, int rowDelta, int colDelta, int64_t& pinned) {
    int r = _row, c = _col;
    int pieceInd = -1;
    while ((r += rowDelta, r >= 0 && r < nrow) && (c += colDelta, c >= 0 && c < ncol)) {
        int ind = c + r * ncol;
        char p = board[ind];
        if (p == '-')
            continue;
        else if (isupper(p) == isWhite) {
            if (pieceInd >= 0)
                return;
            else 
                pieceInd = ind;
        }
        else{
            if ( p == (checkDiag ? (isWhite ? 'b' : 'B') : (isWhite ? 'r' : 'R')) || p == (isWhite ? 'q' : 'Q')) {
                if (pieceInd >= 0) {
                    pinned |= (1LL << pieceInd); 
                }
            }
            return;
        }
    }
}

template<bool isWhite>
void King<isWhite>::getPinnedPieces(const std::string& board, std::vector<int64_t>& pinnedPieces) {
    pinnedPieces.resize(4, 0); // HOR, VER, DIAG1, DIAG2
    pinCheckHelper(board, false,  0, -1, pinnedPieces[0]); // left
    pinCheckHelper(board, false,  0,  1, pinnedPieces[0]); // right
    pinCheckHelper(board, false, -1,  0, pinnedPieces[1]); // down
    pinCheckHelper(board, false,  1,  0, pinnedPieces[1]); // up
    pinCheckHelper(board, true, -1, -1, pinnedPieces[2]); // down-left
    pinCheckHelper(board, true,  1,  1, pinnedPieces[2]); // up-right
    pinCheckHelper(board, true,  1, -1, pinnedPieces[3]); // up-left
    pinCheckHelper(board, true, -1,  1, pinnedPieces[3]); // down-right
}


template void King<true>::getLegalMoves(const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void King<false>::getLegalMoves(const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void King<true>::getPinnedPieces(const std::string& board, std::vector<int64_t>& pinnedPieces);
template void King<false>::getPinnedPieces(const std::string& board, std::vector<int64_t>& pinnedPieces);
template int King<true>::isChecked(const std::string& board, int* checkingPiece);
template int King<false>::isChecked(const std::string& board, int* checkingPiece);
