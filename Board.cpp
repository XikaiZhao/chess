#include "Board.h"
#include <cmath>
#include <stdexcept>

template<bool isWhite>
void Board::makeMove(const Move& move) {
    lastMovePieceChangeID = -1;
    Piece* p = board[move.newPos].get();
    if (p != nullptr) {
        p->_onBoard = false;
        lastMovePieceChangeID = board[move.newPos]->_id;
    }

    lastMoveFromInitPos = board[move.curPos]->_atInitialPos;
    board[move.curPos]->_atInitialPos = false;

    int c = move.newPos%ncol;
    int r = move.newPos/ncol;

    if (move.newPieceType != NONE) {
        int id = board[move.curPos]->_id;
        board[move.newPos] = createPiece<isWhite>(move.newPieceType, r, c, id);
        board[move.newPos]->_atInitialPos = false;
        board[move.curPos] = nullptr;

        std::vector<std::shared_ptr<Piece> >& pieces = (isWhite) ? whitePieces : blackPieces;
        pieces[id] = board[move.newPos];
    }
    else {
        // en passant
        if (board[move.curPos]->_type == PAWN && move.curPos%ncol != c && board[move.newPos] == nullptr) {
            board[lastMove.newPos]->_onBoard = false;
            lastMovePieceChangeID = board[lastMove.newPos]->_id;
            board[lastMove.newPos] = nullptr;     
        }
        board[move.newPos] = board[move.curPos]; // move piece to new cell
        board[move.newPos]->updatePosition(move.newPos); // reset piece position
        board[move.curPos] = nullptr; // remove current piece on the old cell
        
        // castling (move rook)
        if (board[move.newPos]->_type == KING && std::abs(move.curPos%ncol - c) == 2) {
            int rook_c = (ncol - c -1 < c) ? ncol-1 : 0;
            int ind = rook_c + r*ncol;
            int ind_new = (ncol - c -1 < c) ?  move.newPos - 1 : move.newPos + 1;
            board[ind_new] = board[ind];
            board[ind_new]->updatePosition(ind_new);
            board[ind_new]->_atInitialPos = false;
            board[ind] = nullptr;     
        }
    }

    lastMove2 = lastMove;
    lastMove = move;
}

// call makeMove first before calling this function
template<bool isWhite>
void Board::undoLastMove() {    
    int c = lastMove.newPos%ncol;
    int r = lastMove.newPos/ncol;

    if (lastMove.newPieceType != NONE) {
        int id = board[lastMove.newPos]->_id;
        board[lastMove.newPos].reset();
        if (lastMovePieceChangeID >= 0) {
            board[lastMove.newPos] = (isWhite) ? whitePieces[lastMovePieceChangeID] : blackPieces[lastMovePieceChangeID];
            board[lastMove.newPos]->_onBoard = true;
        }
        board[lastMove.curPos] = createPiece<isWhite>(PieceType::PAWN, lastMove.curPos/ncol, lastMove.curPos%ncol, id);
        board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;
        
        std::vector<std::shared_ptr<Piece> >& pieces = (isWhite) ? whitePieces : blackPieces;
        pieces[id] = board[lastMove.curPos];
    
    }
    else {
        board[lastMove.curPos] = board[lastMove.newPos];
        board[lastMove.curPos]->updatePosition(lastMove.curPos);  
        board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;
        board[lastMove.newPos] = nullptr;
        if (lastMovePieceChangeID >= 0) {
            std::vector<std::shared_ptr<Piece> >& pieces = (isWhite) ? blackPieces : whitePieces; 
            pieces[lastMovePieceChangeID]->_onBoard = true;
            board[pieces[lastMovePieceChangeID]->getPosition()] = pieces[lastMovePieceChangeID];
        }  

        // castling (move rook)
        if (board[lastMove.curPos]->_type == KING && std::abs(lastMove.curPos%ncol - c) == 2) {
            int rook_c = (ncol - c -1 < c) ? ncol-1 : 0;
            int ind = rook_c + r*ncol;
            int ind_new = (ncol - c -1 < c) ?  lastMove.newPos - 1 : lastMove.newPos + 1;
            board[ind] = board[ind_new]; 
            board[ind]->updatePosition(ind);
            board[ind]->_atInitialPos = true;
            board[ind_new] = nullptr;   
        }
        
    }
    
    lastMovePieceChangeID = -1;
    lastMove = lastMove2;
}

template void Board::makeMove<true>(const Move& move);
template void Board::makeMove<false>(const Move& move);
template void Board::undoLastMove<true>();
template void Board::undoLastMove<false>();