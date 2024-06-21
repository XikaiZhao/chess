#include "Piece.h"
#include "Board.h"

template<bool isWhite>
void Piece::getLegalMovesHelperFunc(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int dx, int dy) {
    if (pieceIndexKingChecked >= 0) {
        getLegalMovesKingCheckedHelper<isWhite>(ind, pinnedDir, board.getBoard(), legalMoves, board.getKingIndex(isWhite), pieceIndexKingChecked, dx, dy);
    } else {
        getLegalMovesHelper<isWhite>(ind, pinnedDir, board.getBoard(), legalMoves, dx, dy);
    }
}

template<bool isWhite>
void Piece::getLegalMovesHelper(int ind, Piece::PinnedDir pinnedDir, const std::string& board, std::vector<Move>& legalMoves, int rowDelta, int colDelta) {
    int r = ind/ncol + rowDelta;
    int c = ind%ncol + colDelta;
    while (r >= 0 && r < nrow && c >= 0 && c < ncol) {
        int nn = c + r * ncol;
        char p = board[nn];
        if (p == '-') {
            //std::cout << ind <<": Piece is nullptr " << nn << std::endl;
            legalMoves.push_back(Move{ind, nn});
        }
        else if (isupper(p) != isWhite) {
            //std::cout << ind << ": Piece is opposite color " << nn << std::endl;
            legalMoves.push_back(Move{ind, nn});
            break;
        }
        else {
            //std::cout << ind << ": Piece is same color " << nn << std::endl;
            break;
        }
        r += rowDelta;
        c += colDelta;
    }
}

template<bool isWhite>
void Piece::getLegalMovesKingCheckedHelper(int ind, Piece::PinnedDir pinnedDir, const std::string& board, std::vector<Move>& legalMoves, int kingPos, int pieceIndexKingChecked, int rowDelta, int colDelta) {
    int row_king = kingPos / ncol;
    int col_king = kingPos % ncol;

    int row_target = pieceIndexKingChecked / ncol;
    int col_target = pieceIndexKingChecked % ncol;

    int dRow = row_target - row_king;
    int dCol = col_target - col_king;

    const char pp = std::towlower(board[pieceIndexKingChecked]);
    bool needToCapture = (pp == 'p' || pp == 'n');

    int r = ind/ncol + rowDelta;
    int c = ind%ncol + colDelta;
    while (r >= 0 && r < nrow && c >= 0 && c < ncol) {
        int nn = c + r * ncol;
        const char p = board[nn];
        if (pieceIndexKingChecked == nn) {
            legalMoves.push_back(Move{ind, nn});
            break;
        }
        else if ( p != '-') {
            break;
        }
        else if (!needToCapture) {
            if (r >= std::min(row_king, row_target) && r <= std::max(row_king, row_target) &&
                c >= std::min(col_king, col_target) && c <= std::max(col_king, col_target)) 
            {   
                if ((r - row_king)*dCol == (c - col_king)*dRow) {
                    legalMoves.push_back(Move{ind, nn});
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
void Bishop<isWhite>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    Piece::PinnedDir pinnedDir = Piece::getPinnedDir(ind, pinnedPieces);
    
    if (pinnedDir != HOR && pinnedDir != VER) {
        if (pinnedDir == NA || pinnedDir == DIAG1) {
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, -1, -1); // down-left
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  1,  1); // up-right
        }
        if (pinnedDir == NA || pinnedDir == DIAG2) {
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  1, -1); // up-left
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, -1,  1); // down-right
        }
    }
}

///////////////////////////
template<bool isWhite>
void Rook<isWhite>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    Piece::PinnedDir pinnedDir = Piece::getPinnedDir(ind, pinnedPieces);

    if (pinnedDir != DIAG1 && pinnedDir != DIAG2) { 
        if (pinnedDir == NA || pinnedDir == HOR) {
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  0, -1); // left
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  0,  1); // right
        }       
        if (pinnedDir == NA || pinnedDir == VER) {    
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, -1,  0); // down
            Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  1,  0); // up
        }
    }
}

//////////////////////////
template<bool isWhite>
void Queen<isWhite>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    Piece::PinnedDir pinnedDir = Piece::getPinnedDir(ind, pinnedPieces);

    if (pinnedDir == NA || pinnedDir == HOR) {
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, 0, -1); // left
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, 0,  1); // right
    }
    if (pinnedDir == NA || pinnedDir == VER) {
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, -1, 0); // down
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  1, 0); // up
    }
    if (pinnedDir == NA || pinnedDir == DIAG1) {
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, -1, -1); // down-left
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  1,  1); // up-right
    }
    if (pinnedDir == NA || pinnedDir == DIAG2) {
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked,  1, -1); // up-left
        Piece::getLegalMovesHelperFunc<isWhite>(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, -1,  1); // down-right
    }
}

///////////////////////////
template<bool isWhite>
const int Knight<isWhite>::rowDelta[8] = {-1, -1,  1,  1, -2, -2,  2,  2};

template<bool isWhite>
const int Knight<isWhite>::colDelta[8] = {-2,  2, -2,  2, -1,  1, -1,  1};

template<bool isWhite>
void Knight<isWhite>::getLegalMovesHelperFunc(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int dx, int dy) {
    if (pieceIndexKingChecked >= 0) {
        getLegalMovesKingCheckedHelper(ind, pinnedDir, board.getBoard(), legalMoves, board.getKingIndex(isWhite), pieceIndexKingChecked, dx, dy);
    } else {
        getLegalMovesHelper(ind, pinnedDir, board.getBoard(), legalMoves, dx, dy);
    }
}

template<bool isWhite>
void Knight<isWhite>::getLegalMovesHelper(int ind, Piece::PinnedDir pinnedDir, const std::string& board, std::vector<Move>& legalMoves, int /*rowDelta*/, int /*colDelta*/) {
    for (int i = 0; i < 8; i++) {
        int r = ind/ncol + rowDelta[i], c = ind%ncol + colDelta[i];
        if (r >= 0 && r < nrow && c >= 0 && c < ncol) {
            int nn = c + r * ncol;
            //std::cout << posToString(ind) << " " << posToString(nn) << std::endl;
            const char p = board[nn];
            if (p == '-' || isupper(p) != isWhite) {
                legalMoves.push_back(Move{ind, nn});
            }
        }
    }
}

template<bool isWhite>
void Knight<isWhite>::getLegalMovesKingCheckedHelper(int ind, Piece::PinnedDir pinnedDir, const std::string& board, std::vector<Move>& legalMoves, int kingPos, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/) {
    int row_king = kingPos / ncol;
    int col_king = kingPos % ncol;

    int row_target = pieceIndexKingChecked / ncol;
    int col_target = pieceIndexKingChecked % ncol;
    
    int dRow = row_target - row_king;
    int dCol = col_target - col_king;

    const char pp = std::towlower(board[pieceIndexKingChecked]);
    bool needToCapture = (pp == 'p' || pp == 'n');

    for (int i = 0; i < 8; i++) {
        int r = ind/ncol + rowDelta[i], c = ind%ncol + colDelta[i];
        if (r >= 0 && r < nrow && c >= 0 && c < ncol) {
            int nn = c + r * ncol;
            if (pieceIndexKingChecked == nn) {
                legalMoves.push_back(Move{ind, nn});
            }
            else if (!needToCapture && board[nn] == '-'){
                if (r >= std::min(row_king, row_target) && r <= std::max(row_king, row_target) &&
                    c >= std::min(col_king, col_target) && c <= std::max(col_king, col_target)) 
                {   
                    if ((r - row_king)*dCol == (c - col_king)*dRow) {
                        legalMoves.push_back(Move{ind, nn});
                    }
                }
            }
        }
    }
}

template<bool isWhite>
void Knight<isWhite>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    Piece::PinnedDir pinnedDir = getPinnedDir(ind, pinnedPieces);
    if (pinnedDir == NA) {
        getLegalMovesHelperFunc(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, 0, 0);
    }
}

///////////////////////////
template<bool isWhite>
void Pawn<isWhite>::addPromotionMoves(std::vector<Move>& legalMoves, int index, int newPos) {
        legalMoves.push_back(Move{index, newPos, 'b'});
        legalMoves.push_back(Move{index, newPos, 'n'});
        legalMoves.push_back(Move{index, newPos, 'r'});
        legalMoves.push_back(Move{index, newPos, 'q'});
}

template<bool isWhite>
void Pawn<isWhite>::getLegalMovesHelperFunc(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int dx, int dy) {
    if (pieceIndexKingChecked >= 0) {
        getLegalMovesKingCheckedHelper(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, dx, dy);
    } else {
        getLegalMovesHelper(ind, pinnedDir, board, legalMoves, dx, dy);
    }
}

template<bool isWhite>
void Pawn<isWhite>::getLegalMovesHelper(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move> &legalMoves, int /*rowDelta*/, int /*colDelta*/) {
    int col = ind % ncol;
    int row = ind / ncol;
    if (pinnedDir == HOR) return;

    int row_new = (isWhite) ? row+1 : row-1;
    if (row_new >= 0 && row_new < nrow) {
        int nn = col + row_new*ncol;
        if (pinnedDir != DIAG1 &&  pinnedDir != DIAG2) {
            // move straight    
            if (board.getPieceAt(nn) == '-') {
                if (row_new == (isWhite ? 7 : 0)) 
                    addPromotionMoves(legalMoves, ind, nn);
                else 
                    legalMoves.push_back(Move{ind, nn});

                // move 2 squares
                int nn2 = col + (isWhite?row+2:row-2)*ncol;
                if ((isWhite ? row == 1 : row == 6) && board.getPieceAt(nn2) == '-') 
                    legalMoves.push_back(Move{ind, nn2});
            } 
        }
        
        
        if (pinnedDir != VER ) {
            // move diagonal   
            int enPassantCapture = board.getEnPassantCapture();    
            int cc = enPassantCapture%ncol;
            bool canEnPassant = (enPassantCapture != -1 && std::abs(col-cc) == 1 && row_new == (isWhite ? 5 : 2));        
            if (pinnedDir != (isWhite ? DIAG1 : DIAG2)) {
                const char p = board.getPieceAt(nn-1);
                if (col > 0 && p != '-' && isupper(p) != isWhite) {
                    if (row_new == (isWhite ? 7 : 0)) 
                        addPromotionMoves(legalMoves, ind, nn-1);
                    else 
                        legalMoves.push_back(Move{ind, nn-1});
                }
                if (canEnPassant && col > cc) 
                    legalMoves.push_back(Move{ind, cc+row_new*ncol});
            }
            if (pinnedDir != (isWhite ? DIAG2 : DIAG1)) {
                const char p = board.getPieceAt(nn+1);
                if (col+1 < ncol && p != '-' && isupper(p) != isWhite) {
                    if (row_new == (isWhite ? 7 : 0)) 
                        addPromotionMoves(legalMoves, ind, nn+1);
                    else 
                        legalMoves.push_back(Move{ind, nn+1});
                }
                if (canEnPassant && col < cc) 
                    legalMoves.push_back(Move{ind, cc+row_new*ncol});
            }
        }
    }
}


template<bool isWhite>
void Pawn<isWhite>::getLegalMovesKingCheckedHelper(int ind, Piece::PinnedDir pinnedDir, const Board& board, std::vector<Move>& legalMoves, int pieceIndexKingChecked, int /*rowDelta*/, int /*colDelta*/){
    if (pinnedDir == HOR) return;
    int col = ind % ncol;
    int row = ind / ncol;

    int kingPos = board.getKingIndex(isWhite);
    int row_king = kingPos / ncol;
    int col_king = kingPos % ncol;

    int row_target = pieceIndexKingChecked / ncol;
    int col_target = pieceIndexKingChecked % ncol;

    int dRow = row_target - row_king;
    int dCol = col_target - col_king;
    
    const char pp = std::towlower(board.getPieceAt(pieceIndexKingChecked));
    bool needToCapture = (pp == 'p' || pp == 'n');

    auto canStopCheck = [&](int r, int c, bool canCapture) {
        int nn = c + r * ncol;
        if (canCapture && nn == pieceIndexKingChecked) 
            return true;
        else if (needToCapture) 
            return false;

        if (!needToCapture && board.getPieceAt(nn) == '-') {
            if (r >= std::min(row_king, row_target) && r <= std::max(row_king, row_target) &&
                c >= std::min(col_king, col_target) && c <= std::max(col_king, col_target)) 
            {   
                if ((r - row_king)*dCol == (c - col_king)*dRow) 
                    return true;
            }
        }
        return false;
    };

    int row_new = (isWhite) ? row+1 : row-1;
    if (row_new >= 0 && row_new < nrow) {
        int nn = col + row_new*ncol;
        if (pinnedDir != DIAG1 &&  pinnedDir != DIAG2) {
            // move straight
            if (board.getPieceAt(nn) == '-'){
                if (canStopCheck(row_new, col, false)) {
                    if (row_new == (isWhite ? 7 : 0)) 
                        addPromotionMoves(legalMoves, ind, nn);
                    else 
                        legalMoves.push_back(Move{ind, nn});
                }

                // move 2 squares
                int r = (isWhite?row+2:row-2);
                int nn2 = col + r*ncol;
                if ((isWhite ? row == 1 : row == 6) && board.getPieceAt(nn2) == '-' && canStopCheck(r, col, false)) {
                    legalMoves.push_back(Move{ind, nn2});
                }
            } 
        }
             
        if (pinnedDir != VER) {
            // move diagonal
            int enPassantCapture = board.getEnPassantCapture();    
            int cc = enPassantCapture%ncol;
            bool canEnPassant = (enPassantCapture != -1 && std::abs(col-cc) == 1 && row_new == (isWhite ? 5 : 2));        
            if (pinnedDir != (isWhite ? DIAG1 : DIAG2) && col > 0) {  
                char p = board.getPieceAt(nn-1);
                if (p != '-' && isupper(p) != isWhite && canStopCheck(row_new, col-1, true)) {
                    if (row_new == (isWhite ? 7 : 0)) 
                        addPromotionMoves(legalMoves, ind, nn-1);
                    else 
                        legalMoves.push_back(Move{ind, nn-1});
                }
                if (canEnPassant && col > cc) {
                    if (enPassantCapture == pieceIndexKingChecked || canStopCheck(row_new, cc, false))
                        legalMoves.push_back(Move{ind, cc+row_new*ncol});
                }
            }
            if (pinnedDir != (isWhite ? DIAG2 : DIAG1) && col+1 < ncol) {
                char p = board.getPieceAt(nn+1);
                if (p != '-' && isupper(p) != isWhite && canStopCheck(row_new, col+1, true)) {
                    if (row_new == (isWhite ? 7 : 0)) 
                        addPromotionMoves(legalMoves, ind, nn+1);
                    else 
                        legalMoves.push_back(Move{ind, nn+1});
                }
                if (canEnPassant && col < cc) {
                    if (enPassantCapture == pieceIndexKingChecked || canStopCheck(row_new, cc, false))
                        legalMoves.push_back(Move{ind, cc+row_new*ncol});
                }
            }
        }
    }
}

template<bool isWhite>
void Pawn<isWhite>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked) {
    Piece::PinnedDir pinnedDir = getPinnedDir(ind, pinnedPieces);
    getLegalMovesHelperFunc(ind, pinnedDir, board, legalMoves, pieceIndexKingChecked, 0, 0);
}


///////////////////////////
template void Pawn<true>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Pawn<false>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Rook<true>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Rook<false>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Bishop<true>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Bishop<false>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Queen<true>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Queen<false>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Knight<true>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
template void Knight<false>::getLegalMoves(int ind, std::vector<int64_t>& pinnedPieces, const Board& board, std::vector<Move >& legalMoves, int pieceIndexKingChecked);
