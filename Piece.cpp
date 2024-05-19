#include "Piece.h"
#include "Board.h"

template<bool isWhite>
void Piece::getLegalMovesHelperFunc(Board* board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int dx, int dy) {
    if (pieceIndexKingChecked >= 0) {
        getLegalMovesKingCheckedHelper<isWhite>(board, legalMoves, pieceIndexKingChecked, dx, dy);
    } else {
        getLegalMovesHelper<isWhite>(board, legalMoves, dx, dy);
    }
}

template<bool isWhite>
void Piece::getLegalMovesHelper(Board* board, std::vector<Move>& legalMoves, int rowDelta, int colDelta) {
    int r = _row + rowDelta;
    int c = _col + colDelta;
    while (r >= 0 && r < nrow && c >= 0 && c < ncol) {
        int nn = c + r * ncol;
        const Piece* p = board->getPiece(nn);
        if (p == nullptr) {
            //std::cout << _ind <<": Piece is nullptr " << nn << std::endl;
            legalMoves.push_back(Move{_ind, nn});
        }
        else if (p->_isWhite != isWhite) {
            //std::cout << _ind << ": Piece is opposite color " << nn << std::endl;
            legalMoves.push_back(Move{_ind, nn});
            break;
        }
        else {
            //std::cout << _ind << ": Piece is same color " << nn << std::endl;
            break;
        }
        r += rowDelta;
        c += colDelta;
    }
}

template<bool isWhite>
void Piece::getLegalMovesKingCheckedHelper(Board* board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int rowDelta, int colDelta) {
    int kingPos = isWhite ? board->whiteKingPos : board->blackKingPos;
    int row_king = kingPos / ncol;
    int col_king = kingPos % ncol;

    int row_target = pieceIndexKingChecked / ncol;
    int col_target = pieceIndexKingChecked % ncol;

    int dRow = row_target - row_king;
    int dCol = col_target - col_king;

    const Piece* p = board->getPiece(pieceIndexKingChecked);
    bool needToCapture = (p->_type == PieceType::PAWN || p->_type == PieceType::KNIGHT);

    int r = _row + rowDelta;
    int c = _col + colDelta;
    while (r >= 0 && r < nrow && c >= 0 && c < ncol) {
        int nn = c + r * ncol;
        const Piece* p = board->getPiece(nn);
        if (pieceIndexKingChecked == nn) {
            legalMoves.push_back(Move{_ind, nn});
            break;
        }
        else if ( p!= nullptr) {
            break;
        }
        else if (!needToCapture && board->getPiece(nn) == nullptr) {
            if (r >= std::min(row_king, row_target) && r <= std::max(row_king, row_target) &&
                c >= std::min(col_king, col_target) && c <= std::max(col_king, col_target)) 
            {   
                if ((r - row_king)*dCol == (c - col_king)*dRow) {
                    legalMoves.push_back(Move{_ind, nn});
                    break;
                }
            }
        }
        r += rowDelta;
        c += colDelta;
    }
}

///////////////////////////
template<bool isWhite>
void Bishop<isWhite>::getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    if (_onBoard && _isPinned != HOR && _isPinned != VER) {
        if (_isPinned == NA || _isPinned == DIAG1) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, -1, -1); // down-left
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  1,  1); // up-right
        }
        if (_isPinned == NA || _isPinned == DIAG2) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  1, -1); // up-left
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, -1,  1); // down-right
        }
    }
    _isPinned = NA;
}

///////////////////////////
template<bool isWhite>
void Rook<isWhite>::getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    if (_onBoard && _isPinned != DIAG1 && _isPinned != DIAG2) { 
        if (_isPinned == NA || _isPinned == HOR) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  0, -1); // left
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  0,  1); // right
        }       
        if (_isPinned == NA || _isPinned == VER) {    
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, -1,  0); // down
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  1,  0); // up
        }
    }
    _isPinned = NA;
}

//////////////////////////
template<bool isWhite>
void Queen<isWhite>::getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    if (_onBoard) {
        if (_isPinned == NA || _isPinned == HOR) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, 0, -1); // left
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, 0,  1); // right
        }
        if (_isPinned == NA || _isPinned == VER) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, -1, 0); // down
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  1, 0); // up
        }
        if (_isPinned == NA || _isPinned == DIAG1) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, -1, -1); // down-left
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  1,  1); // up-right
        }
        if (_isPinned == NA || _isPinned == DIAG2) {
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked,  1, -1); // up-left
            getLegalMovesHelperFunc<isWhite>(board, legalMoves, pieceIndexKingChecked, -1,  1); // down-right
        }
    }
    _isPinned = NA;
}

///////////////////////////
template<bool isWhite>
const int Knight<isWhite>::rowDelta[8] = {-1, -1,  1,  1, -2, -2,  2,  2};

template<bool isWhite>
const int Knight<isWhite>::colDelta[8] = {-2,  2, -2,  2, -1,  1, -1,  1};

template<bool isWhite>
void Knight<isWhite>::getLegalMovesHelperFunc(Board* board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int dx, int dy) {
    if (pieceIndexKingChecked >= 0) {
        getLegalMovesKingCheckedHelper(board, legalMoves, pieceIndexKingChecked, dx, dy);
    } else {
        getLegalMovesHelper(board, legalMoves, dx, dy);
    }
}

template<bool isWhite>
void Knight<isWhite>::getLegalMovesHelper(Board* board, std::vector<Move>& legalMoves, int /*rowDelta*/, int /*colDelta*/) {
    for (int i = 0; i < 8; i++) {
        int r = _row + rowDelta[i], c = _col + colDelta[i];
        if (r >= 0 && r < nrow && c >= 0 && c < ncol) {
            int nn = c + r * ncol;
            //std::cout << posToString(_ind) << " " << posToString(nn) << std::endl;
            Piece* p = board->getPiece(nn);
            if (p == nullptr || p->_isWhite != isWhite) {
                legalMoves.push_back(Move{_ind, nn});
            }
        }
    }
}

template<bool isWhite>
void Knight<isWhite>::getLegalMovesKingCheckedHelper(Board* board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/) {
    int kingPos = isWhite ? board->whiteKingPos : board->blackKingPos;
    int row_king = kingPos / ncol;
    int col_king = kingPos % ncol;

    int row_target = pieceIndexKingChecked / ncol;
    int col_target = pieceIndexKingChecked % ncol;
    
    int dRow = row_target - row_king;
    int dCol = col_target - col_king;

    const Piece* p = board->getPiece(pieceIndexKingChecked);
    bool needToCapture = (p->_type == PieceType::PAWN || p->_type == PieceType::KNIGHT);

    for (int i = 0; i < 8; i++) {
        int r = _row + rowDelta[i], c = _col + colDelta[i];
        int nn = c + r * ncol;
        if (pieceIndexKingChecked == nn) {
            legalMoves.push_back(Move{_ind, nn});
        }
        else if (!needToCapture && board->getPiece(nn) == nullptr){
            if (r >= std::min(row_king, row_target) && r <= std::max(row_king, row_target) &&
                c >= std::min(col_king, col_target) && c <= std::max(col_king, col_target)) 
            {   
                if ((r - row_king)*dCol == (c - col_king)*dRow) {
                    legalMoves.push_back(Move{_ind, nn});
                    break;
                }
            }
        }
    }
}

template<bool isWhite>
void Knight<isWhite>::getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    if (_onBoard && _isPinned == NA) {
        getLegalMovesHelperFunc(board, legalMoves, pieceIndexKingChecked, 0, 0);
    }
    _isPinned = NA;
}

///////////////////////////
template<bool isWhite>
void Pawn<isWhite>::addPromotionMoves(std::vector<Move>& legalMoves, int index, int newPos) {
        legalMoves.push_back(Move{index, newPos, PieceType::BISHOP});
        legalMoves.push_back(Move{index, newPos, PieceType::KNIGHT});
        legalMoves.push_back(Move{index, newPos, PieceType::ROOK});
        legalMoves.push_back(Move{index, newPos, PieceType::QUEEN});
}

template<bool isWhite>
void Pawn<isWhite>::getLegalMovesHelperFunc(Board* board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int dx, int dy) {
    if (pieceIndexKingChecked >= 0) {
        getLegalMovesKingCheckedHelper(board, legalMoves, pieceIndexKingChecked, dx, dy);
    } else {
        getLegalMovesHelper(board, legalMoves, dx, dy);
    }
}

template<bool isWhite>
void Pawn<isWhite>::getLegalMovesHelper(Board*board, std::vector<Move> &legalMoves, int /*rowDelta*/, int /*colDelta*/) {
    int row_new = (isWhite) ? _row+1 : _row-1;
    if (row_new >= 0 && row_new < nrow) {
        // move straight
        int nn = _col + row_new*ncol;
        if (board->getPiece(nn) == nullptr) {
            if (row_new == (isWhite ? 7 : 0)) 
                addPromotionMoves(legalMoves, _ind, nn);
            else 
                legalMoves.push_back(Move{_ind, nn});
        } 
        
        if (_isPinned == NA) {
            // move diagonal
            Piece* p = board->getPiece(nn-1);
            if (_col > 0 && p != nullptr && p->_isWhite != isWhite) {
                if (row_new == (isWhite ? 7 : 0)) 
                    addPromotionMoves(legalMoves, _ind, nn-1);
                else 
                    legalMoves.push_back(Move{_ind, nn-1});
            }
            p = board->getPiece(nn+1);
            if (_col+1 < ncol && p != nullptr && p->_isWhite != isWhite) {
                if (row_new == (isWhite ? 7 : 0)) 
                    addPromotionMoves(legalMoves, _ind, nn+1);
                else 
                    legalMoves.push_back(Move{_ind, nn+1});
            }
        }
    }

    if (_isPinned == NA) {
        // en passant
        const Move& lastMove = board->getLastMove();
        if (_row == lastMove.newPos/ncol && std::abs(_col - lastMove.newPos%ncol) == 1 &&
            board->getPiece(lastMove.newPos)->_type == PieceType::PAWN && 
            std::abs(lastMove.curPos/ncol-lastMove.newPos/ncol) == 2) 
        {
            legalMoves.push_back(Move{_ind, lastMove.newPos%ncol+row_new*ncol});
        }
    }

    // move 2 squares
    int nn = _col + (isWhite?_row+2:_row-2)*ncol;
    if ((isWhite ? _row == 1 : _row == 6) && board->getPiece(nn) == nullptr) {
        legalMoves.push_back(Move{_ind, nn});
    }
}


template<bool isWhite>
void Pawn<isWhite>::getLegalMovesKingCheckedHelper(Board* board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/){
    int kingPos = isWhite ? board->whiteKingPos : board->blackKingPos;
    int row_king = kingPos / ncol;
    int col_king = kingPos % ncol;

    int row_target = pieceIndexKingChecked / ncol;
    int col_target = pieceIndexKingChecked % ncol;

    int dRow = row_target - row_king;
    int dCol = col_target - col_king;

    const Piece* p = board->getPiece(pieceIndexKingChecked);
    bool needToCapture = (p->_type == PieceType::PAWN || p->_type == PieceType::KNIGHT);

    auto canBlockCheck = [&](int r, int c, bool canCapture) {
        if (needToCapture && !canCapture) 
            return false;

        int nn = c + r * ncol;
        if (needToCapture && canCapture && nn == pieceIndexKingChecked) 
            return true;

        if (!needToCapture && board->getPiece(nn) == nullptr){
            if (r >= std::min(row_king, row_target) && r <= std::max(row_king, row_target) &&
                c >= std::min(col_king, col_target) && c <= std::max(col_king, col_target)) 
            {   
                if ((r - row_king)*dCol == (c - col_king)*dRow) 
                    return true;
            }
        }
        return false;
    };

    int row_new = (isWhite) ? _row+1 : _row-1;
    if (row_new >= 0 && row_new < nrow) {
        // move straight
        int nn = _col + row_new*ncol;
        if (board->getPiece(nn) == nullptr) {
            if (canBlockCheck(row_new, _col, false)) {
                if (row_new == (isWhite ? 7 : 0)) 
                    addPromotionMoves(legalMoves, _ind, nn);
                else 
                    legalMoves.push_back(Move{_ind, nn});
            }
        } 
             
        if (_isPinned == NA) {
            // move diagonal
            Piece* p = board->getPiece(nn-1);
            if (_col > 1 && p != nullptr && p->_isWhite != isWhite && canBlockCheck(row_new, _col-1, true)) {
                if (row_new == (isWhite ? 7 : 0)) 
                    addPromotionMoves(legalMoves, _ind, nn-1);
                else 
                    legalMoves.push_back(Move{_ind, nn-1});
            }
            p = board->getPiece(nn+1);
            if (_col+1 < ncol && p != nullptr && p->_isWhite != isWhite && canBlockCheck(row_new, _col+1, true)) {
                if (row_new == (isWhite ? 7 : 0)) 
                    addPromotionMoves(legalMoves, _ind, nn+1);
                else 
                    legalMoves.push_back(Move{_ind, nn+1});
            }
        }
    }

    if (_isPinned == NA)
    {    
        // en passant
        const Move& lastMove = board->getLastMove();
        if (_row == lastMove.newPos/ncol && std::abs(_col - lastMove.newPos%ncol) == 1 &&
            board->getPiece(lastMove.newPos)->_type == PAWN && 
            std::abs(lastMove.curPos/ncol-lastMove.newPos/ncol) == 2) 
        {
            if (lastMove.newPos == pieceIndexKingChecked)
                legalMoves.push_back(Move{_ind, lastMove.newPos%ncol+row_new*ncol});
            else if (canBlockCheck(row_new, lastMove.newPos%ncol, false))
                legalMoves.push_back(Move{_ind, lastMove.newPos%ncol+row_new*ncol});
        }
    }

    // move 2 squares
    int r = (isWhite?_row+2:_row-2);
    int nn = _col + r*ncol;
    if ((isWhite ? _row == 1 : _row == 6) && board->getPiece(nn) == nullptr) {
        if (canBlockCheck(r, _col, false))
            legalMoves.push_back(Move{_ind, nn});
    }
}

template<bool isWhite>
void Pawn<isWhite>::getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    if (_onBoard && (_isPinned == NA || _isPinned == VER)) {
        getLegalMovesHelperFunc(board, legalMoves, pieceIndexKingChecked, 0, 0);
    }
    _isPinned = NA;
}

//////////////////////////
template<bool isWhite>
int King<isWhite>::isCheckedHelper1(Board* board, PieceType type, int rowDelta, int colDelta, int* checkingPiece) {
    int r = _row + rowDelta;
    int c = _col + colDelta;

    if (r >= 0 && r < nrow && c >= 0 && c < ncol) {
        int nn = c + r * ncol;
        Piece* p = board->getPiece(nn);
        if (p != nullptr && p->_type == type && p->_isWhite != isWhite) {
            if (checkingPiece != nullptr) *checkingPiece = nn;
            return 1;
        }
    }
    return 0;
}

template<bool isWhite>
int King<isWhite>::isCheckedHelper2(Board* board, bool checkdiag, int rowDelta, int colDelta, int* checkingPiece) {
    int r = _row, c = _col;
    while ((r += rowDelta, r >= 0 && r < nrow) && (c += colDelta, c >= 0 && c < ncol)) {
        int ind = c + r * ncol;
        const Piece* p = board->getPiece(ind);
        if (p == nullptr)
            continue;
        else if (p->_isWhite == isWhite) {
            return 0;
        }
        else{
            if ( p->_type == (checkdiag ? PieceType::BISHOP : PieceType::ROOK) || p->_type == PieceType::QUEEN) {
                if (checkingPiece != nullptr) *checkingPiece = ind;
                return 1;
            }
        }
    }
    return 0;
}

template<bool isWhite>
int  King<isWhite>::isChecked(Board* board, int* checkingPiece) {  
        if (board == nullptr) {
            throw std::runtime_error("Board is null");
        }
    int res = 0;

    // check pawn
    res += isCheckedHelper1(board, PieceType::PAWN, isWhite?1:-1, -1, checkingPiece);
    res += isCheckedHelper1(board, PieceType::PAWN, isWhite?1:-1,  1, checkingPiece);
    if (!checkingPiece && res > 0)
        return res;
    
    // check knight 
    res += isCheckedHelper1(board, PieceType::KNIGHT, -1, -2, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT, -1,  2, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT,  1, -2, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT,  1,  2, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT, -2, -1, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT, -2,  1, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT,  2, -1, checkingPiece);
    res += isCheckedHelper1(board, PieceType::KNIGHT,  2,  1, checkingPiece);
    if (!checkingPiece && res > 0)
        return res;

    // check bishop and queen
    res += isCheckedHelper2(board, true, -1, -1, checkingPiece); // down-left
    res += isCheckedHelper2(board, true,  1,  1, checkingPiece); // up-right
    res += isCheckedHelper2(board, true,  1, -1, checkingPiece); // up-left
    res += isCheckedHelper2(board, true, -1,  1, checkingPiece); // down-right
    if (!checkingPiece && res > 0)
        return res;

    // check rook and queen
    res += isCheckedHelper2(board, false,  0, -1, checkingPiece); // left
    res += isCheckedHelper2(board, false,  0,  1, checkingPiece); // right
    res += isCheckedHelper2(board, false, -1,  0, checkingPiece); // down
    res += isCheckedHelper2(board, false,  1,  0, checkingPiece); // up
    if (!checkingPiece && res > 0)
        return res;

    return res;
}

template<bool isWhite>
void King<isWhite>::getLegalMovesHelper(Board* board, std::vector<Move>& legalMoves, int rowDelta, int colDelta) {
    int newRow = _row + rowDelta;
    int newCol = _col + colDelta;

    if (newRow >= 0 && newRow < nrow && newCol >= 0 && newCol < ncol) {
        int pos = newCol + newRow * ncol;
        const Piece* p = board->getPiece(pos);
        if (p != nullptr && p->_isWhite == isWhite)
            return;
            
        int opponentKingPos = isWhite ? board->blackKingPos : board->whiteKingPos;
        if (opponentKingPos != pos - ncol - 1 && opponentKingPos != pos - ncol && opponentKingPos != pos - ncol + 1 &&
            opponentKingPos != pos - 1 && opponentKingPos != pos + 1 &&
            opponentKingPos != pos + ncol - 1 && opponentKingPos != pos + ncol && opponentKingPos != pos + ncol + 1) 
        {
            Move move{_ind, pos};
            board->makeMove<isWhite>(move);
            if (!isChecked(board, nullptr)) 
                legalMoves.push_back(move);
            board->undoLastMove<isWhite>();
        }
    }
}

template<bool isWhite>
void King<isWhite>::getLegalMoves(Board* board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    getLegalMovesHelper(board, legalMoves, -1, 0); // Up
    getLegalMovesHelper(board, legalMoves, 1, 0); // Down
    getLegalMovesHelper(board, legalMoves, 0, -1); // Left
    getLegalMovesHelper(board, legalMoves, 0, 1); // Right
    getLegalMovesHelper(board, legalMoves, -1, -1); // Up-left
    getLegalMovesHelper(board, legalMoves, -1, 1); // Up-right
    getLegalMovesHelper(board, legalMoves, 1, -1); // Down-left
    getLegalMovesHelper(board, legalMoves, 1, 1); // Down-right
    if (pieceIndexKingChecked >= 0 || !_atInitialPos)
        return;

    // castling
    int cols[2] = {0, 7};
    int colDelta[2] = {-1, 1};
    for (int i = 0; i < 2; i++) {
        // check if rook is at initial position
        const Piece* p = board->getPiece(cols[i] + _row*ncol);
        bool canCastle = (p != nullptr && p->_atInitialPos);
        // check if there are pieces between king and rook
        for (int c = _col; canCastle && (c += colDelta[i]) > 0 && c < ncol-1; canCastle = (board->getPiece(c + _row*ncol) == nullptr));
        // check if king will pass through check
        for (int c = _col; canCastle && (c += colDelta[i]) > 1 && c < ncol-1; ) {
            board->makeMove<isWhite>(Move{_ind, c + _row*ncol});
            canCastle = (isChecked(board, nullptr) == 0);
            board->undoLastMove<isWhite>();
        }
        // add castling move
        if (canCastle)
            legalMoves.push_back(Move{_ind, _col+ colDelta[i]*2 + _row*ncol});
    }
}

template<bool isWhite>
void King<isWhite>::pinCheckHelper(Board* board, bool checkDiag, PinnedDirection dir, int rowDelta, int colDelta) {
    int r = _row, c = _col;
    int pieceInd = -1;
    while ((r += rowDelta, r >= 0 && r < nrow) && (c += colDelta, c >= 0 && c < ncol)) {
        int ind = c + r * ncol;
        const Piece* p = board->getPiece(ind);
        if (p == nullptr)
            continue;
        else if (p->_isWhite == isWhite) {
            if (pieceInd >= 0)
                return;
            else 
                pieceInd = ind;
        }
        else{
            if ( p->_type == (checkDiag ? PieceType::BISHOP : PieceType::ROOK) || p->_type == PieceType::QUEEN) {
                if (pieceInd >= 0) 
                    board->getPiece(pieceInd)->_isPinned = dir; 
            }
            return;
        }
    }
}

template<bool isWhite>
void King<isWhite>::updatePinnedPieces(Board* board) {
    pinCheckHelper(board, false, HOR,  0, -1); // left
    pinCheckHelper(board, false, HOR,  0,  1); // right
    pinCheckHelper(board, false, VER, -1,  0); // down
    pinCheckHelper(board, false, VER,  1,  0); // up
    pinCheckHelper(board, true, DIAG1,-1, -1); // down-left
    pinCheckHelper(board, true, DIAG1, 1,  1); // up-right
    pinCheckHelper(board, true, DIAG2, 1, -1); // up-left
    pinCheckHelper(board, true, DIAG2,-1,  1); // down-right
}

////////////////////////
template class Bishop<true>;
template class Bishop<false>;
template class Rook<true>;
template class Rook<false>;
template class Queen<true>;
template class Queen<false>;
template class Knight<true>;
template class Knight<false>;
template class Pawn<true>;
template class Pawn<false>;
template class King<true>;
template class King<false>;
